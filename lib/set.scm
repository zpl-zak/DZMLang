(define (entry tree) (car tree))
(define (left-branch tree) (cadr tree))
(define (right-branch tree) (caddr tree))
(define (make-tree entry left right)
  (list entry left right))

(define (element-of-set? x s)
  (cond ((null? s) #f)
        ((= x (entry s)) #t)
        ((< x (entry s))
         (element-of-set?
          x
          (left-branch s)))
        ((> x (entry s))
         (element-of-set?
          x
          (right-branch s)))))

(define (element-of-set-list? x set)
  (cond ((null? set) false)
        ((= x (car set)) true)
        ((< x (car set)) false)
        (else (element-of-set-list? x (cdr set)))))

(define (adjoin-set x set)
  (cond ((null? set) (make-tree x '() '()))
        ((= x (entry set)) set)
        ((< x (entry set))
         (make-tree
          (entry set)
          (adjoin-set x (left-branch set))
          (right-branch set)))
        ((> x (entry set))
         (make-tree
          (entry set)
          (left-branch set)
          (adjoin-set x (right-branch set))))))

(define (partial-tree elts n)
  (if (= n 0)
      (cons '() elts)
      (let ((left-size
             (quotient (- n 1) 2)))
        (let ((left-result
               (partial-tree
                elts left-size)))
          (let ((left-tree
                 (car left-result))
                (non-left-elts
                 (cdr left-result))
                (right-size
                 (- n (+ left-size 1))))
            (let ((this-entry
                   (car non-left-elts))
                  (right-result
                   (partial-tree
                    (cdr non-left-elts)
                    right-size)))
              (let ((right-tree
                     (car right-result))
                    (remaining-elts
                     (cdr right-result)))
                (cons (make-tree this-entry
                                 left-tree
                                 right-tree)
                      remaining-elts))))))))

(define (list->tree elements)
  (car (partial-tree
        elements (length elements))))

(define (adjoin-set-list x set)
  (cond ((null? set) (list x))
        ((= x (car set)) set)
        ((< x (car set)) (cons x set))
        (else (cons (car set) (adjoin-set-list x (cdr set))))))

(define (union-set-list set1 set2)
  (cond  ((null? set1) set2)
         ((null? set2) set1)
         (else
          (let ((x1 (car set1))
                (x2 (car set2)))
            (cond ((= x1 x2) (cons x1 (union-set-list (cdr set1) (cdr set2))))
                  ((< x1 x2) (cons x1 (union-set-list (cdr set1) set2)))
                  (else (cons x2 (union-set-list set1 (cdr set2)))))))))

(define (union-set tree1 tree2)
  (list->tree (union-set-list (tree->list tree1)
                              (tree->list tree2))))

(define (intersection-set-list set1 set2)
  (if (or (null? set1) (null? set2))
      '()
      (let ((x1 (car set1)) (x2 (car set2)))
        (cond ((= x1 x2)
               (cons x1 (intersection-set
                         (cdr set1)
                         (cdr set2))))
              ((< x1 x2) (intersection-set
                          (cdr set1)
                          set2))
              ((< x2 x1) (intersection-set
                          set1
                          (cdr set2)))))))

(define (intersection-set tree1 tree2)
  (list->tree (intersection-set-list (tree->list tree1)
                                     (tree->list tree2))))

(define (tree->list tree)
  (if (null? tree)
      '()
      (append
       (tree->list
        (left-branch tree))
       (cons (entry tree)
             (tree->list
              (right-branch tree))))))

(define (lookup-unordered given-key set-of-records)
  (cond ((null? set-of-records) false)
        ((equal? given-key
                 (key (car set-of-records)))
         (car set-of-records))
        (else
         (lookup given-key
                 (cdr set-of-records)))))

(define (lookup given-key set-of-records)
  (cond ((null? set-of-records) #f)
        ((= given-key (key (entry set-of-records)))
         (entry set-of-records))
        ((< given-key (key (entry set-of-records)))
         (lookup given-key (left-branch set-of-records)))
        (else (lookup given-key (right-branch set-of-records)))))

