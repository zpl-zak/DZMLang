(define (make-account balance)
  (define (withdraw amount)
    (if (>= balance amount)
        (begin (set! balance
                     (- balance amount))
               balance)
        "Insufficient funds"))
  (define (deposit amount)
    (set! balance (+ balance amount))
    balance)
  (define (dispatch m)
    (cond ((eq? m 'withdraw) withdraw)
          ((eq? m 'deposit) deposit)
          (else (error "Unknown request:
                 MAKE-ACCOUNT" m))))
  dispatch)

(define (make-monitored f)
  (let ((count 0))
    (lambda (arg)
      (cond ((eq? arg 'how-many-calls?) count)
            ((eq? arg 'reset-counter) (set! count 0))
            (else (begin (set! count (+ count 1))
                         (apply f arg)))))))


(define (make-point c-x c-y)
  (let (
        (x c-x)
        (y c-y))

    (define (get-x) x)
    (define (get-y) y)

    (define (type-of) 'point)

    (define (self m)
      (cond ((eq? m 'get-x) get-x)
            ((eq? m 'get-y) get-y)
            ((eq? m 'type-of) type-of)
            (else (no-msg))))
    self))

(define (make-color-point c-x c-y c-color)
  (let (
        (super (make-point c-x c-y))
        (color c-color))

    (define (get-color) color)

    (define (type-of) 'color-point)

    (define (self m)
      (cond ((eq? m 'get-color) get-color)
            ((eq? m 'type-of) type-of)
            (else (super m))))
    self))

(define mjoe 10)
(define mser (make-serializer))
(define (msett x y)
  (begin
    (sleep y)
    (set! mjoe x)))
