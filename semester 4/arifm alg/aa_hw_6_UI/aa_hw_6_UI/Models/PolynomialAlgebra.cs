using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;

namespace PolynomialAlgebra
{
    public enum TermOrderType
    {
        Lex,
        GrLex,
        GrevLex,
        InvLex,
        RInvLex
    }

    public static class TermOrder
    {
        public static int Compare(int[] x, int[] y, TermOrderType type)
        {
            return type switch
            {
                TermOrderType.Lex => CompareLex(x, y),
                TermOrderType.GrLex => CompareGrLex(x, y),
                TermOrderType.GrevLex => CompareGrevLex(x, y),
                TermOrderType.InvLex => CompareInvLex(x, y),
                TermOrderType.RInvLex => CompareRInvLex(x, y),
                _ => CompareLex(x, y)
            };
        }

        private static int CompareLex(int[] x, int[] y)
        {
            for (int i = 0; i < x.Length; i++)
            {
                if (x[i] != y[i]) return y[i].CompareTo(x[i]); // Сортировка по убыванию
            }
            return 0;
        }

        private static int CompareGrLex(int[] x, int[] y)
        {
            int degX = x.Sum();
            int degY = y.Sum();
            if (degX != degY) return degY.CompareTo(degX);
            return CompareLex(x, y);
        }

        private static int CompareGrevLex(int[] x, int[] y)
        {
            int degX = x.Sum();
            int degY = y.Sum();
            if (degX != degY) return degY.CompareTo(degX);
            
            for (int i = x.Length - 1; i >= 0; i--)
            {
                // В GrevLex при равной степени бОльшим считается тот терм, у которого меньше степень последней переменной
                if (x[i] != y[i]) return x[i].CompareTo(y[i]); 
            }
            return 0;
        }

        private static int CompareInvLex(int[] x, int[] y)
        {
            for (int i = x.Length - 1; i >= 0; i--)
            {
                if (x[i] != y[i]) return y[i].CompareTo(x[i]);
            }
            return 0;
        }

        private static int CompareRInvLex(int[] x, int[] y)
        {
            int degX = x.Sum();
            int degY = y.Sum();
            if (degX != degY) return degY.CompareTo(degX);
            return CompareInvLex(x, y);
        }
    }

    // Структура для возврата термов
    public readonly record struct Term(int[] Exps, BigInteger Coeff);

    public class Polynomial : IEquatable<Polynomial>
    {
        // Обязательное условие: хранение в Trie
        private class Node
        {
            public BigInteger Coeff { get; set; }
            public Dictionary<int, Node> Children { get; }

            public Node(BigInteger coeff = default)
            {
                Coeff = coeff;
                Children = new Dictionary<int, Node>();
            }

            public Node Clone()
            {
                var clone = new Node(Coeff);
                foreach (var kvp in Children)
                {
                    clone.Children[kvp.Key] = kvp.Value.Clone();
                }
                return clone;
            }
        }

        private Node root;
        private readonly string[] vars;
        public TermOrderType OrderType { get; set; }

        public IReadOnlyList<string> Variables => vars;

        public Polynomial(IEnumerable<string> variables, TermOrderType orderType = TermOrderType.Lex)
        {
            vars = variables.ToArray();
            if (vars.Length == 0) throw new ArgumentException("Требуется хотя бы одна переменная.");
            root = new Node();
            OrderType = orderType;
        }

        public Polynomial(Polynomial other)
        {
            root = other.root.Clone();
            vars = (string[])other.vars.Clone();
            OrderType = other.OrderType;
        }

        public bool IsZero() => root.Coeff.IsZero && root.Children.Count == 0;

        public void AddTerm(int[] exps, BigInteger coeff)
        {
            if (exps.Length != vars.Length)
                throw new ArgumentException("Количество степеней не совпадает с количеством переменных.");
            
            if (exps.Any(e => e < 0))
                throw new ArgumentException("Отрицательные степени не допускаются.");

            Insert(root, exps, 0, coeff);
            CleanNode(root);
        }

        private void Insert(Node node, int[] exps, int depth, BigInteger value)
        {
            if (value.IsZero) return;

            if (depth == exps.Length)
            {
                node.Coeff += value;
                return;
            }

            int e = exps[depth];
            if (!node.Children.TryGetValue(e, out Node child))
            {
                child = new Node();
                node.Children[e] = child;
            }

            Insert(child, exps, depth + 1, value);
        }

        // Ленивая генерация термов (существенная экономия памяти)
        public IEnumerable<Term> Supp()
        {
            var cur = new List<int>(vars.Length);
            foreach (var term in Collect(root, cur))
            {
                yield return term;
            }
        }

        private IEnumerable<Term> Collect(Node node, List<int> cur)
        {
            if (node == null) yield break;

            if (!node.Coeff.IsZero)
            {
                yield return new Term(cur.ToArray(), node.Coeff);
            }

            foreach (var kv in node.Children)
            {
                cur.Add(kv.Key);
                foreach (var res in Collect(kv.Value, cur))
                {
                    yield return res;
                }
                cur.RemoveAt(cur.Count - 1);
            }
        }

        public IReadOnlyList<Term> GetSortedTerms()
        {
            var terms = Supp().ToList();
            terms.Sort((a, b) => TermOrder.Compare(a.Exps, b.Exps, OrderType));
            return terms;
        }

        public Term LeadingTerm()
        {
            var terms = GetSortedTerms();
            return terms.Count > 0 ? terms[0] : new Term(new int[vars.Length], BigInteger.Zero);
        }

        #region Арифметика дерева (Trie)

        private void AddNodes(Node a, Node b)
        {
            if (b == null) return;
            a.Coeff += b.Coeff;

            foreach (var kv in b.Children)
            {
                if (!a.Children.TryGetValue(kv.Key, out Node childA))
                {
                    a.Children[kv.Key] = kv.Value.Clone();
                }
                else
                {
                    AddNodes(childA, kv.Value);
                    if (childA.Coeff.IsZero && childA.Children.Count == 0)
                    {
                        a.Children.Remove(kv.Key);
                    }
                }
            }
        }

        private void SubNodes(Node a, Node b)
        {
            if (b == null) return;
            a.Coeff -= b.Coeff;

            foreach (var kv in b.Children)
            {
                if (!a.Children.TryGetValue(kv.Key, out Node childA))
                {
                    var tmp = kv.Value.Clone();
                    Negate(tmp);
                    a.Children[kv.Key] = tmp;
                }
                else
                {
                    SubNodes(childA, kv.Value);
                    if (childA.Coeff.IsZero && childA.Children.Count == 0)
                    {
                        a.Children.Remove(kv.Key);
                    }
                }
            }
        }

        private void Negate(Node node)
        {
            if (node == null) return;
            node.Coeff = -node.Coeff;
            foreach (var child in node.Children.Values)
            {
                Negate(child);
            }
        }

        private void Scale(Node node, BigInteger scalar)
        {
            if (node == null) return;
            node.Coeff *= scalar;
            foreach (var child in node.Children.Values)
            {
                Scale(child, scalar);
            }
        }

        private bool CleanNode(Node node)
        {
            if (node == null) return true;

            var toErase = new List<int>();
            foreach (var kv in node.Children)
            {
                if (CleanNode(kv.Value))
                {
                    toErase.Add(kv.Key);
                }
            }

            foreach (int key in toErase)
            {
                node.Children.Remove(key);
            }

            return node.Coeff.IsZero && node.Children.Count == 0;
        }

        #endregion

        #region Перегрузка операторов

        public static Polynomial operator +(Polynomial a, Polynomial b)
        {
            CheckCompatibility(a, b);
            var result = new Polynomial(a);
            result.AddNodes(result.root, b.root);
            result.CleanNode(result.root);
            return result;
        }

        public static Polynomial operator -(Polynomial a)
        {
            var result = new Polynomial(a);
            result.Negate(result.root);
            return result;
        }

        public static Polynomial operator -(Polynomial a, Polynomial b)
        {
            CheckCompatibility(a, b);
            var result = new Polynomial(a);
            result.SubNodes(result.root, b.root);
            result.CleanNode(result.root);
            return result;
        }

        public static Polynomial operator *(Polynomial a, BigInteger scalar)
        {
            var result = new Polynomial(a);
            result.Scale(result.root, scalar);
            result.CleanNode(result.root);
            return result;
        }

        public static Polynomial operator *(BigInteger scalar, Polynomial a) => a * scalar;

        public static Polynomial operator *(Polynomial a, Polynomial b)
        {
            CheckCompatibility(a, b);
            var result = new Polynomial(a.vars, a.OrderType);

            foreach (var termA in a.Supp())
            {
                foreach (var termB in b.Supp())
                {
                    int[] newExps = new int[a.vars.Length];
                    for (int i = 0; i < newExps.Length; i++)
                    {
                        newExps[i] = termA.Exps[i] + termB.Exps[i];
                    }
                    BigInteger newCoeff = termA.Coeff * termB.Coeff;
                    
                    if (!newCoeff.IsZero)
                    {
                        result.AddTerm(newExps, newCoeff);
                    }
                }
            }
            return result;
        }

        #endregion

        #region Деление и вычисления

        public (Polynomial[] Quotients, Polynomial Remainder) Divide(Polynomial[] divisors)
        {
            if (divisors == null || divisors.Length == 0)
                throw new ArgumentException("Требуется как минимум один делитель.");

            foreach (var div in divisors)
                CheckCompatibility(this, div);

            var quotients = new Polynomial[divisors.Length];
            for (int i = 0; i < divisors.Length; i++)
                quotients[i] = new Polynomial(vars, OrderType);

            var remainder = new Polynomial(vars, OrderType);
            var p = new Polynomial(this);

            while (!p.IsZero())
            {
                bool divisionOccurred = false;
                for (int i = 0; i < divisors.Length && !divisionOccurred; i++)
                {
                    var pLT = p.LeadingTerm();
                    var divLT = divisors[i].LeadingTerm();

                    bool divisible = true;
                    for (int j = 0; j < pLT.Exps.Length; j++)
                    {
                        if (pLT.Exps[j] < divLT.Exps[j])
                        {
                            divisible = false;
                            break;
                        }
                    }

                    if (divisible)
                    {
                        int[] expDiff = new int[pLT.Exps.Length];
                        for (int j = 0; j < pLT.Exps.Length; j++)
                            expDiff[j] = pLT.Exps[j] - divLT.Exps[j];

                        BigInteger coeff = pLT.Coeff / divLT.Coeff;
                        if (coeff * divLT.Coeff != pLT.Coeff)
                            throw new ArgumentException("Деление не является точным: ведущие коэффициенты не делятся нацело.");

                        var term = new Polynomial(vars, OrderType);
                        term.AddTerm(expDiff, coeff);

                        quotients[i] += term;
                        p -= term * divisors[i];
                        divisionOccurred = true;
                    }
                }

                if (!divisionOccurred)
                {
                    var pLT = p.LeadingTerm();
                    var ltPoly = new Polynomial(vars, OrderType);
                    ltPoly.AddTerm(pLT.Exps, pLT.Coeff);
                    remainder += ltPoly;
                    p -= ltPoly;
                }
            }

            return (quotients, remainder);
        }

        public BigInteger Evaluate(BigInteger[] point)
        {
            if (point.Length != vars.Length)
                throw new ArgumentException("Размерность точки не совпадает с количеством переменных.");

            return EvaluateNode(root, point, 0, BigInteger.One);
        }

        private BigInteger EvaluateNode(Node node, BigInteger[] point, int depth, BigInteger accum)
        {
            BigInteger result = BigInteger.Zero;

            if (!node.Coeff.IsZero)
                result += node.Coeff * accum;

            if (depth < point.Length)
            {
                foreach (var kv in node.Children)
                {
                    BigInteger powVal = BigInteger.Pow(point[depth], kv.Key);
                    BigInteger val = accum * powVal;
                    result += EvaluateNode(kv.Value, point, depth + 1, val);
                }
            }

            return result;
        }

        #endregion

        private static void CheckCompatibility(Polynomial a, Polynomial b)
        {
            if (a.vars.Length != b.vars.Length || !a.vars.SequenceEqual(b.vars))
                throw new ArgumentException("Многочлены должны иметь одинаковые имена переменных.");
            
            if (a.OrderType != b.OrderType)
                throw new ArgumentException("Многочлены должны иметь одинаковый тип сортировки термов.");
        }

        public bool Equals(Polynomial other)
        {
            if (other is null) return false;
            if (ReferenceEquals(this, other)) return true;
            if (!vars.SequenceEqual(other.vars)) return false;

            var thisTerms = Supp().ToList();
            var otherTerms = other.Supp().ToList();

            if (thisTerms.Count != otherTerms.Count) return false;

            // Поскольку порядок в Dictionary не гарантирован, мы сравниваем отсортированные термы
            var thisSorted = GetSortedTerms();
            var otherSorted = other.GetSortedTerms();

            for (int i = 0; i < thisSorted.Count; i++)
            {
                if (!thisSorted[i].Exps.SequenceEqual(otherSorted[i].Exps) || thisSorted[i].Coeff != otherSorted[i].Coeff)
                    return false;
            }

            return true;
        }

        public override bool Equals(object obj) => Equals(obj as Polynomial);

        public override int GetHashCode()
        {
            int hash = 17;
            foreach (var v in vars) hash = hash * 31 + v.GetHashCode();
            return hash;
        }

        public override string ToString()
        {
            var terms = GetSortedTerms();
            if (terms.Count == 0) return "0";

            var sb = new StringBuilder();
            bool first = true;

            foreach (var term in terms)
            {
                var exps = term.Exps;
                var coeff = term.Coeff;

                bool negative = coeff.Sign < 0;
                
                if (!first)
                {
                    sb.Append(negative ? " - " : " + ");
                }
                else
                {
                    if (negative) sb.Append("-");
                    first = false;
                }

                BigInteger absC = BigInteger.Abs(coeff);
                bool allExpsZero = exps.All(e => e == 0);
                bool needCoeff = !(absC.IsOne && !allExpsZero);

                if (needCoeff)
                {
                    sb.Append(absC);
                }

                bool firstVar = true;
                for (int i = 0; i < exps.Length; i++)
                {
                    if (exps[i] > 0)
                    {
                        if (!firstVar) sb.Append("*");
                        
                        sb.Append(vars[i]);
                        if (exps[i] > 1) sb.Append("^").Append(exps[i]);
                        
                        firstVar = false;
                    }
                }
            }

            return sb.ToString();
        }
    }
}