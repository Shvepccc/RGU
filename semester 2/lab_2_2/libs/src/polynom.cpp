#define _CRT_SECURE_NO_WARNINGS
#include "../polynom.h"
#include "../../libs/bigfloat.h"
#include "../../../common_libs/helpers.h"

polynom::polynom() 
{
	poly_node new_node;
	poly_list.push(new_node);
}

bigfloat parse_ratio(const char* number_buf, int sign)
{
    const char* slash_pos = std::strchr(number_buf, '/');
    if (slash_pos)
    {
        char numerator_buf[64], denominator_buf[64];
        std::strncpy(numerator_buf, number_buf, slash_pos - number_buf);
        numerator_buf[slash_pos - number_buf] = '\0';
        std::strcpy(denominator_buf, slash_pos + 1);

        bigfloat numerator = bigfloat(convert_TO_decimal(numerator_buf, 10));
        bigfloat denominator = bigfloat(convert_TO_decimal(denominator_buf, 10));
        return sign * (numerator / denominator);
    }
    else
    {
        return sign * bigfloat(convert_TO_decimal(number_buf, 10));
    }
}

polynom::polynom(const char* expr)
{
    int i = 0;
    int len = std::strlen(expr);

    while (i < len)
    {
        while (i < len && std::isspace(expr[i])) ++i;

        int sign = 1;
        if (expr[i] == '+') { sign = 1; ++i; }
        else if (expr[i] == '-') { sign = -1; ++i; }

        char number_buf[128];
        int number_buf_index = 0;

        while (i < len && (std::isdigit(expr[i]) || expr[i] == '.' || expr[i] == '/')) {
            number_buf[number_buf_index++] = expr[i++];
        }
        number_buf[number_buf_index] = '\0';

        bool has_number = (number_buf_index > 0);
        bigfloat ratio = has_number 
            ? parse_ratio(number_buf, sign) 
            : bigfloat(sign);

        unsigned int power = 0;

        if (i < len && expr[i] == 'x')
        {
            power = 1;
            ++i;
            if (i < len && expr[i] == '^')
            {
                ++i;
                char pow_buf[16];
                int pb_idx = 0;
                while (i < len && std::isdigit(expr[i])) {
                    pow_buf[pb_idx++] = expr[i++];
                }
                pow_buf[pb_idx] = '\0';
                power = convert_TO_decimal(pow_buf, 10);
            }
        }
        else if (!has_number) {
            continue;
        }

        poly_list.push(p_node(ratio, power));
    }
    standart_appearance(*this);
}

polynom::~polynom() {}

polynom& polynom::operator+= (polynom const& arg) 
{
    t_list_node<poly_node>* temp_ptr_1 = this->poly_list.get_head();
    t_list_node<poly_node>* temp_ptr_2 = arg.poly_list.get_head();

    t_list<poly_node> result;
    poly_node* n1 = nullptr, * n2 = nullptr;

    while (temp_ptr_1 != nullptr || temp_ptr_2 != nullptr)
    {
        n1 = temp_ptr_1 ? &temp_ptr_1->data : nullptr;
        n2 = temp_ptr_2 ? &temp_ptr_2->data : nullptr;

        if (n1 && n2 && (n1->pow == n2->pow))
        {
            if (n1->ratio + n2->ratio != bigfloat(0, 1))
            {
                result.push(poly_node(n1->ratio + n2->ratio, n1->pow));
            }
            if (temp_ptr_1 != nullptr) temp_ptr_1 = temp_ptr_1->next;
            if (temp_ptr_2 != nullptr) temp_ptr_2 = temp_ptr_2->next;
        }
        else
        {
            if (n1 && (!n2 || n1->pow > n2->pow))
            {
                result.push(*n1);
                if (temp_ptr_1 != nullptr) temp_ptr_1 = temp_ptr_1->next;
            }
            else if (n2)
            {
                result.push(*n2);
                if (temp_ptr_2 != nullptr) temp_ptr_2 = temp_ptr_2->next;
            }
        }
    }
    if (result.get_head() == nullptr)
    {
        result.push(poly_node(0, 0));
    }

    this->poly_list = result;
    standart_appearance(*this);
    return *this;

}

polynom& polynom::operator-= (polynom const& arg) 
{
    poly_node* n1 = nullptr, * n2 = nullptr;
    t_list_node<poly_node>* temp_ptr_1 = this->poly_list.get_head();
    t_list_node<poly_node>* temp_ptr_2 = arg.poly_list.get_head();

    t_list<poly_node> result;

    while (temp_ptr_1 != nullptr || temp_ptr_2 != nullptr)
    {
        n1 = temp_ptr_1 ? &temp_ptr_1->data : nullptr;
        n2 = temp_ptr_2 ? &temp_ptr_2->data : nullptr;


        if (n1 && n2 && (n1->pow == n2->pow))
        {
            if (n1->ratio - n2->ratio != bigfloat(0, 1))
            {
                result.push(poly_node(n1->ratio - n2->ratio, n1->pow));
            }
            if (temp_ptr_1 != nullptr) temp_ptr_1 = temp_ptr_1->next;
            if (temp_ptr_2 != nullptr) temp_ptr_2 = temp_ptr_2->next;
        }
        else
        {
            if (n1 && (!n2 || n1->pow > n2->pow))
            {
                result.push(*n1);
                if (temp_ptr_1 != nullptr) temp_ptr_1 = temp_ptr_1->next;
            }
            else if (n2)
            {
                result.push(*n2);
                if (temp_ptr_2 != nullptr) temp_ptr_2 = temp_ptr_2->next;
            }
        }
    }
    if (result.get_head() == nullptr)
    {
        result.push(poly_node(0, 0));
    }

    this->poly_list = result;
    standart_appearance(*this);
    return *this;
}

polynom& polynom::operator*= (polynom const& arg) 
{
    t_list<poly_node> result;
    poly_node* n1 = nullptr, * n2 = nullptr;

    t_list_node<poly_node>* temp_ptr_1 = this->poly_list.get_head();
    while (temp_ptr_1 != nullptr)
    {
    	n1 = &temp_ptr_1->data;
    
        t_list_node<poly_node>* temp_ptr_2 = arg.poly_list.get_head();
    	while (temp_ptr_2 != nullptr) 
        {
    		n2 = &temp_ptr_2->data;
            result.push(poly_node(n1->ratio * n2->ratio, n1->pow + n2->pow));    
    		temp_ptr_2 = temp_ptr_2->next;
    	}
    	temp_ptr_1 = temp_ptr_1->next;
    }
    this->poly_list = result;
    standart_appearance(*this);
    return *this;
}

polynom& polynom::operator/= (const polynom& arg)
{
    if (arg.poly_list.get_head() == nullptr || arg.poly_list.get_head()->data.ratio == 0)
        throw std::runtime_error("Math error: Divizion by zero");

    polynom dividend = *this;
    polynom result;

    while (true)
    {
        auto head1 = dividend.poly_list.get_head();
        auto head2 = arg.poly_list.get_head();
        if (!head1 || head1->data.pow < head2->data.pow)
            break;

        poly_node lead1 = head1->data;
        poly_node lead2 = head2->data;

        poly_node q_term(lead1.ratio / lead2.ratio, lead1.pow - lead2.pow);
        result.poly_list.push(q_term);

        polynom temp;
        temp.poly_list.push(q_term);
        polynom to_subtract = arg;
        to_subtract *= temp;

        dividend -= to_subtract;
    }

    *this = result;
    standart_appearance(*this);
    return *this;
}

polynom polynom::operator+ (polynom const& arg) const 
{
    polynom copy = *this;
    return copy += arg;
}

polynom polynom::operator- (polynom const& arg) const 
{
    polynom copy = *this;
    return copy -= arg;
}

polynom polynom::operator* (polynom const& arg) const 
{
    polynom copy = *this;
    return copy *= arg;
}

polynom polynom::operator/ (polynom const& arg) const 
{
    polynom copy = *this;
    return copy /= arg;
}

polynom polynom::diff() const
{
    polynom result;
    poly_node* n1 = nullptr;
    t_list_node<poly_node>* temp_ptr_1 = this->poly_list.get_head();
    poly_node temp_node;

    while (temp_ptr_1 != nullptr)
    {
        n1 = &temp_ptr_1->data;

        if (n1->pow == 1)
        {
            temp_node = poly_node(n1->ratio, 0);
        }
        else
        {
            temp_node = poly_node(n1->ratio * n1->pow, n1->pow - 1);
        }

        if (n1->pow != 0) result.poly_list.push(temp_node);
        temp_ptr_1 = temp_ptr_1->next;
    }
    standart_appearance(result);
    return result;
}

polynom polynom::intgr() const
{
    polynom result;
    t_list_node<poly_node>* temp_ptr_1 = this->poly_list.get_head();

    while (temp_ptr_1 != nullptr)
    {
        const poly_node& n1 = temp_ptr_1->data;

        bigfloat new_ratio = n1.ratio / (n1.pow + 1);
        unsigned int new_pow = n1.pow + 1;

        result.poly_list.push(poly_node(new_ratio, new_pow));
        temp_ptr_1 = temp_ptr_1->next;
    }

    standart_appearance(result);
    return result;
}

bool polynom::operator== (polynom const& arg) const
{
    poly_node* n1 = nullptr, * n2 = nullptr;
    t_list_node<poly_node>* temp_ptr_1 = this->poly_list.get_head();
    t_list_node<poly_node>* temp_ptr_2 = arg.poly_list.get_head();

    while (temp_ptr_1 || temp_ptr_2)
    {
        n1 = &temp_ptr_1->data;
        n2 = &temp_ptr_2->data;

        if ((n1->pow != n2->pow) || (n1->ratio != n2->ratio) || !temp_ptr_1 || !temp_ptr_2)
            return false;

        temp_ptr_1 = temp_ptr_1->next;
        temp_ptr_2 = temp_ptr_2->next;
    }
    return true;
}

bool polynom::operator!= (polynom const& arg) const
{
    return !(*this == arg);
}

std::ostream& operator<<(
    std::ostream& stream,
    polynom const& value)
{
    t_list_node<poly_node>* current = value.poly_list.get_head();
    bool first = true;

    while (current) {
        bigfloat coeff = current->data.ratio;
        unsigned int power = current->data.pow;

        if (coeff == 0) {
            current = current->next;
            continue;
        }

        if (!first) {
            if (coeff > 0) stream << " + ";
            else stream << " - ";
        }
        else {
            if (coeff < 0) stream << "-";
        }

        bigfloat abs_coeff = coeff < 0 ? -coeff : coeff;

        if (power == 0) {
            stream << abs_coeff;
        }
        else if (power == 1) {
            if (abs_coeff == 1) stream << "x";
            else stream << abs_coeff << "x";
        }
        else {
            if (abs_coeff == 1) stream << "x^" << power;
            else stream << abs_coeff << "x^" << power;
        }

        first = false;
        current = current->next;
    }

    if (first) stream << "0";

    stream << std::endl;
    return stream;
}

std::istream& operator>>(
    std::istream& stream,
    polynom& value)
{
    char buff[BUFSIZ];
    stream >> buff;
    value = polynom(buff);
    return stream;
}

int comparator_by_pow(poly_node const& a, poly_node const& b) {
	return b.pow - a.pow;
}

void standart_appearance(polynom & arg)
{
    arg.poly_list.sort(comparator_by_pow);

    t_list<p_node> result;
    t_list_node<p_node>* current = arg.poly_list.get_head();

    while (current) {
        p_node sum_node = current->data;
        t_list_node<p_node>* next = current->next;

        while (next && next->data.pow == sum_node.pow) {
            sum_node.ratio += next->data.ratio;
            next = next->next;
        }

        if (sum_node.ratio != 0) {
            result.push(sum_node);
        }

        while (current && current->data.pow == sum_node.pow) {
            current = current->next;
        }
    }

    arg.poly_list = result;
}