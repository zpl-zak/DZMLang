(define (square-list items)
    (map square items))

(define (sum-odd-squares-redundant tree)
  (cond ((null? tree) 0)
        ((not (pair? tree))
         (if (odd? tree) (square tree) 0))
        (else (+ (sum-odd-squares 
                  (car tree))
                 (sum-odd-squares 
                  (cdr tree))))))

(define (sum-odd-squares tree)
    (accumulate
        + 0
        (map square
             (filter odd?
                     (for tree)))))

(define the-x (list (list 1 2) (list 3 4)))

(define (square-tree tree)
    (map (lambda (sub-tree)
             (if (list? sub-tree)
                 (square-tree sub-tree)
                 (square sub-tree)))
             tree))

(define (salary-of-highest-paid-programmer
            records)
        (define (salary r) (cdr r))
        (define (programmer? r) (eq? "programmer" (car r)))
        (accumulate
            max
            0
            (map salary
                 (filter programmer? records))))
             
(define employees (list
                      (list "programmer" 2480)
                      (list "programmer" 560)
                      (list "plumber" 220)))
                  

(define (point params)
 (let ((x (car params)) 
       (y (cdr params))
      )
     
   (define (getx) x)

   (define (gety) y)
   
   (define (test w) (* w w))

   (define (add p) 
    (point 
     (list (+ x ((p 'getx))))
           (+ y ((p 'gety)))))

   (define (type-of) 'point)
     
   (define (self message)
     (cond ((eq? message 'getx) getx)
           ((eq? message 'gety) gety)
           ((eq? message 'add)  add)
           ((eq? message 'test) test)
           ((eq? message 'type-of) type-of)
	   (else (error "Undefined message" message))))
     
   self))
 
 (define (color-point params)
   (let ((super (new point (list (car params) (cdr params))))
                (self 'nil))
              (let ((color (nth 3 params)))
                   
                   (define (get-color) color)
                   (define (type-of) 'color-point)
                   
                   (define (dispatch msg)
                     (cond ((eq? msg 'get-color) get-color)
                           ((eq? msg 'type-of) type-of)
                           (else (super msg))))
                         
                         (set! self dispatch))
                       self))
 
 
 (define p (new point '(1 2)))
 (define q (new point '(3 4)))