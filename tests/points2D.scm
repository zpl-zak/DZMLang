(define (make-point x y)
  (cons x y))

(define (make-vector a b)
  (cons (- (car b) (car a))
        (- (cdr b) (cdr a))))

(define (magnitude u)
  (sqrt (+ (square (car u)) (square (cdr u)))))

(define (multiply u v)
  (+ (* (car u) (car v)) (* (cdr u) (cdr v))))

(define (cosine u v)
  (/ (multiply u v) (* (magnitude u) (magnitude v))))
