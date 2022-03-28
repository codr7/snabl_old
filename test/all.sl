(test 42 42)


(test 3 (+ 1 2))

(test 2 (- 3 1))


(test 2 (let [n 3] (dec n)))

(test 1 (let [n 3] (dec n 2)))


(test 42 (let [foo 35 bar (+ foo 7)] bar))


(test 42
  (let [foo (let [bar 42]
              (fun: baz [] Int bar)
              baz)
        bar 0]
    (foo)))
    

(test 55

  (fun: fib-rec [n Int] Int
    (if (< n 2) n (+ (fib-rec (dec n)) (fib-rec (dec n 2)))))

  (fib-rec 10))

(test 55

  (fun: fib-tail [n Int a Int b Int] Int
    (if (z? n) a (if (one? n) b (fib-tail (dec n) b (+ a b)))))

  (fib-tail 10 0 1))
