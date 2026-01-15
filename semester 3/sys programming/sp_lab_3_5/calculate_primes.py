prime_cache = []
last_checked = 1

def is_prime(n):
    if n < 2:
        return False
    if n == 2:
        return True
    if n % 2 == 0:
        return False
    i = 3
    while i * i <= n:
        if n % i == 0:
            return False
        i += 2
    return True

def get_n_primes(n):
    global prime_cache, last_checked
    
    if len(prime_cache) >= n:
        return prime_cache[:n]
    
    while len(prime_cache) < n:
        if is_prime(last_checked):
            prime_cache.append(last_checked)
        last_checked += 1
    
    return prime_cache[:n]

def primeorial_prime(n):
    primes = get_n_primes(n)
    product = 1
    for prime in primes:
        product *= prime
    return product + 1

with open('primeorial_primes.txt', 'w') as f:
    for i in range(1, 53):
        result = primeorial_prime(i)
        f.write(str(result) + '\n')