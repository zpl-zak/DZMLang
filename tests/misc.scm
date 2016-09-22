(define (is-power-of x y)
  (if (= x y)
      #t
      (if (< y x)
          #f
          (is-power-of x (sqrt y)))))
