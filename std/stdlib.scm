(define number? integer?)
(define list? pair?)
(define nil '())

(define (caar x) (car (car x)))
(define (cadr x) (car (cdr x)))
(define (cdar x) (cdr (car x)))
(define (cddr x) (cdr (cdr x)))
(define (caaar x) (car (car (car x))))
(define (caadr x) (car (car (cdr x))))
(define (cadar x) (car (cdr (car x))))
(define (caddr x) (car (cdr (cdr x))))
(define (cdaar x) (cdr (car (car x))))
(define (cdadr x) (cdr (car (cdr x))))
(define (cddar x) (cdr (cdr (car x))))
(define (cdddr x) (cdr (cdr (cdr x))))
(define (caaaar x) (car (car (car (car x)))))
(define (caaadr x) (car (car (car (cdr x)))))
(define (caadar x) (car (car (cdr (car x)))))
(define (caaddr x) (car (car (cdr (cdr x)))))
(define (cadaar x) (car (cdr (car (car x)))))
(define (cadadr x) (car (cdr (car (cdr x)))))
(define (caddar x) (car (cdr (cdr (car x)))))
(define (cadddr x) (car (cdr (cdr (cdr x)))))
(define (cdaaar x) (cdr (car (car (car x)))))
(define (cdaadr x) (cdr (car (car (cdr x)))))
(define (cdadar x) (cdr (car (cdr (car x)))))
(define (cdaddr x) (cdr (car (cdr (cdr x)))))
(define (cddaar x) (cdr (cdr (car (car x)))))
(define (cddadr x) (cdr (cdr (car (cdr x)))))
(define (cdddar x) (cdr (cdr (cdr (car x)))))
(define (cddddr x) (cdr (cdr (cdr (cdr x)))))

(define (string->list l)
  (if (nil? l)
    '()
    (cons (car l) (string->list (cdr l)))))

(define (list->string l)
  (if (nil? l)
    '()
    (concat (car l) (list->string (cdr l)))))
(define conc concat)

(define (length items)
  (define (iter a count)
    (if (null? a)
        count
        (iter (cdr a) (+ 1 count))))
  (iter items 0))

(define (attach-tag type-tag contents)
  (cons type-tag contents))

(define (type-tag x)
  (if (pair? x)
      (car x)
      (error "Bad tagged entity: TYPE-TAG" x)))

(define (contents x)
  (if (pair? x)
      (cdr x)
      (error "Bad tagged entity: CONTENT" x)))



(define (count-leaves x)
  (cond ((null? x) 0)
        ((not (pair? x)) 1)
        (else (+ (count-leaves (car x))
                 (count-leaves (cdr x))))))

(define (append list1 list2)
  (if (null? list1)
      list2
      (cons (car list1) (append (cdr list1) list2))))

(define (reverse l)
  (define (iter in out)
    (if (pair? in)
        (iter (cdr in) (cons (car in) out))
        out))
  (iter l '()))

(define (deep-reverse ls)
  (define (iter ls acc)
    (if (null? ls)
        acc
        (if (list? (car ls))
            (iter (cdr ls) (cons (deep-reverse (car ls)) acc))
            (iter (cdr ls) (cons (car ls) acc)))))
  (iter ls '()))

(define (flatten x)
  (cond ((null? x) '())
        ((pair? x) (append (flatten (car x)) (flatten (cdr x))))
        (else (list x))))

(define (reverse-string l)
  (list->string (reverse (string->list l))))

(define (map proc items)
    (if (null? items)
        '()
        (cons (proc (car items))
              (map proc (cdr items)))))

(define (tree-map proc tree)
    (map (lambda (sub-tree)
             (if (list? sub-tree)
                 (tree-map proc sub-tree)
                 (proc sub-tree)))
             tree))

(define (subsets ls)
  (if (null? ls)
      (list nil)
      (let ((rest (subsets (cdr ls))))
           (append rest (map (lambda (x)
                               (cons (car ls) x))
                             rest)))))

(define (for-each f l)
  (if (null? l)
      #t
      (begin
        (f (car l))
        (for-each f (cdr l)))))

(define (repeat f n)
  (if (null? n)
      #t
      (begin
        (f)
        (repeat f (cdr n)))))

(define (contains x l)
  (cond ((nil? l) #f)
        ((eq? x (car l)) #t)
        (else (contains x (cdr l)))))

(define (filter predicate sequence)
  (cond ((null? sequence) nil)
        ((predicate (car sequence))
                    (cons (car sequence)
                          (filter predicate
                                  (cdr sequence))))
                    (else (filter predicate
                                  (cdr sequence)))))

(define (accumulate op initial sequence)
  (if (null? sequence)
      initial
      (op (car sequence)
          (accumulate op
                      initial
                      (cdr sequence)))))
(define acc accumulate)

(define (enumerate-interval low high)
  (if (> low high)
      nil
      (cons low
            (enumerate-interval
              (+ low 1)
              high))))
(define range enumerate-interval)

(define (enumerate-tree tree)
  (cond ((null? tree) nil)
        ((not (list? tree)) (list tree))
        (else (append
                (enumerate-tree (car tree))
                (enumerate-tree (cdr tree))))))
(define for enumerate-tree)

(define (foldr func end lst)
  (if (null? lst)
      end
      (func (car lst) (foldr func end (cdr lst)))))

(define (foldl func accum lst)
  (if (null? lst)
      accum
      (foldl func (func accum (car lst)) (cdr lst))))

(define fold foldl)
(define reduce foldr)

(define (unfold func init pred)
  (if (pred init)
      (cons init '())
      (cons init (unfold func (func init) pred))))

(define (max lst)
  (fold (lambda (old new)
          (if (> old new)
              old
              new))
            (car lst)
            (cdr lst)))

(define (min lst)
  (fold (lambda (old new)
          (if (< old new)
              old
              new))
            (car lst)
            (cdr lst)))

(define (id o) o)
(define (flip f) (lambda (a b) (f b a)))

(define (curry f a) (lambda (b) (apply f (cons a (list b)))))
(define (compose f g) (lambda (a) (f (apply g a))))

(define zero?              (curry = 0))
(define positive?          (curry < 0))
(define negative?          (curry > 0))
(define (odd? num)         (= (mod num 2) 1))
(define (even? num)        (= (mod num 2) 0))

(define (send message obj params)
  (cond
   ((or (null? obj) (null? message)) '())
   ((null? params) (apply (obj message)))
   (else (apply (obj message) params))))

(define (new class params)
  (cond
   ((null? params) (class))
   (else (apply class params))))

(define (not x)
  (if x #f #t))

(define (take n l)
  (if (or (nil? l) (= 0 n))
    '()
    (cons (car l) (take (- n 1) (cdr l)))))

(define (last l)
  (if (nil? (cdr l))
    (car l)
    (last (cdr l))))

(define (last-pair l)
  (if (nil? (cdr l))
      l
      (last-pair (cdr l))))

(define (no-more? l)
  (if (null? (car (cdr l)))
      #t
      #f))

(define (nth n l)
  (last (take n l)))

(define (remove n l)
  (filter (lambda (x) (not (eq? x n)))
          l))

(define (memq n x)
  (cond ((null? x) #f)
        ((eq? n (car x)) x)
        (else (memq n (cdr x)))))

(define (apply-generic op args)
  (let ((type-tags (map type-tag args)))
    (let ((proc (get op type-tags)))
      (if proc
          (apply proc (map contents args))
          (error
           "No method for these types:
             APPLY-GENERIC"
           (list op type-tags))))))

;;; QUEUE
 (define (make-queue)
   (let ((front-ptr '())
         (rear-ptr '()))
         (define (empty-queue?) (null? front-ptr))
         (define (set-front-ptr! item) (set! front-ptr item))
         (define (set-rear-ptr! item) (set! rear-ptr item))
         (define (front-queue)
           (if (empty-queue?)
             (error "FRONT called with an empty queue")
             (car front-ptr)))
         (define (insert-queue! item)
           (let ((new-pair (cons item '())))
             (cond ((empty-queue?)
                     (set-front-ptr! new-pair)
                     (set-rear-ptr! new-pair))
                   (else
                     (set-cdr! rear-ptr new-pair)
                     (set-rear-ptr! new-pair)))))
         (define (delete-queue!)
           (cond ((empty-queue?)
                   (error "DELETE called with an empty queue"))
                 (else (set-front-ptr! (cdr front-ptr)))))
         (define (print-queue) front-ptr)
         (define (dispatch m)
           (cond ((eq? m 'empty) empty-queue?)
                 ((eq? m 'front) front-queue)
                 ((eq? m 'insert!) insert-queue!)
                 ((eq? m 'delete!) delete-queue!)
                 ((eq? m 'print) print-queue)
                 ((eq? m 'type-of) (lambda() 'queue))
                 (else (error "undefined operation -- QUEUE" m))))
         dispatch))

;;; SET

(define (make-set)
  (let ((set '()))
    (define (insert! x)
      (if (null? set) (set! set (list x))
          (insert-iter x set)))
    (define (insert-iter x l)
      (cond ((null? l) (list x))
            ((= x (car l)) #t)
            ((< x (car l)) (set! set (cons x l)))
            (else (set! set (cons (car l) (insert-iter x (cdr l)))))))
    (define (get) set)
    (define (self m)
      (cond ((eq? m 'insert!) insert!)
            ((eq? m 'get) get)
            ((eq? m 'type-of) (lambda() 'set))
            (else (no-msg))))
      self))

;;; TABLE

(define (fold-left op init seq)
   (define (iter ans rest)
     (if (null? rest)
         ans
         (iter (op ans (car rest))
               (cdr rest))))
   (iter init seq))

 (define (make-table same-key?)
   (define (associate key records)
     (cond ((null? records) '())
           ((same-key? key (caar records)) (car records))
           (else (associate key (cdr records)))))

   (let ((the-table (list '*table*)))
     (define (lookup keys)
       (define (lookup-record record-list key)
         (if (not (null? record-list))
             (let ((record (associate key record-list)))
               (if (null? record)
                   '()
                   (cdr record)))
             '()))
       (fold-left lookup-record (cdr the-table) keys))

     (define (insert! keys value)
       (define (descend table key)
         (let ((record (associate key (cdr table))))
           (if (not (null? record))
               record
               (let ((new (cons (list key)
                                (cdr table))))
                 (set-cdr! table new)
                 (car new)))))
       (set-cdr! (fold-left descend the-table keys)
                 value))

     (define (print)
       (define (indent tabs)
         (for-each (lambda (x) (display "-")) (range 0 tabs)))

       (define (print-record rec level)
         (indent level)
         (display (car rec))
         (display ": ")
         (if (list? (cdr rec))
             (begin (newline)
                    (print-table rec (+ level 1)))
             (begin (display (cdr rec))
                    (newline))))
       (define (print-table table level)
         (if (null? (cdr table))
             (begin (display "-no entries-")
                    (newline))
             (for-each (lambda (record)
                         (print-record record level))
                       (cdr table))))
       (print-table the-table 0))

     (define (dispatch m)
       (cond ((eq? m 'lookup) lookup)
             ((eq? m 'update!) insert!)
             ((eq? m 'insert!) insert!)
             ((eq? m 'print) print)
             ((eq? m 'the-table) the-table)
             ((eq? m 'type-of) (lambda() 'table))
             (else (error "Undefined method" m))))
     dispatch))

;;; MULTI-THREADING

(define spawn parallel-exec)
(define (make-serializer)
  (let ((mutex (make-mutex)))
    (define (serialized-p p args)
      (begin
        (mutex 'acquire)
        (let ((val (apply p args)))
          (begin
            (mutex 'release)
            val))))
    serialized-p))

(define (make-mutex)
  (let ((cell (list #f)))
    (define (the-mutex m)
      (cond ((eq? m 'acquire)
             (if (eq? (test-and-set! cell) #t)
                 (the-mutex 'acquire)
                 #f))
            ((eq? m 'release) (clear! cell))))
    the-mutex))

(define (clear! cell)
  (set-car! cell false)
  #t)

(define (make-semaphore maximum-clients) 
  (let ((access-mutex (make-mutex)) 
        (exceeded-mutex (make-mutex)) 
        (clients 0)) 
    (define (the-semaphore message) 
      (cond ((eq? message 'acquire) 
             (access-mutex 'acquire) 
             (cond ((> clients maximum-clients) 
                    (access-mutex 'release) 
                    (exceeded-mutex 'acquire) 
                    (the-semaphore 'acquire)) 
                   (else 
                    (set! clients (+ clients 1)) 
                    (if (= clients maximum-clients) 
                        (exceeded-mutex 'acquire)) 
                    (access-mutex 'release)))) 
            ((eq? message 'release) 
             (access-mutex 'acquire) 
             (set! clients (- clients 1)) 
             (exceeded-mutex 'release) 
             (access-mutex 'release)))) 
    the-semaphore)) 

(define (test-and-set! lock)
  (let ((initial (car lock)))
    (begin
      (set-car! lock #t)
      (if (eq? initial #t) #t #f))))

;;; GENERIC

(define (char-lowercase x)
  (if (and (>= (char->integer x) 65) (<= (char->integer x) 90))
      (integer->char (+ (char->integer x) 32))
      x))

(define (char-uppercase x)
  (if (and (>= (char->integer x) 97) (<= (char->integer x) 122))
      (integer->char (- (char->integer x) 32))
      x))

(define (char-flipcase x)
  (if (and (>= (char->integer x) 65) (<= (char->integer x) 90))
      (char-lowercase x)
      (char-uppercase x)))

(define op-table (make-table eq?))
(define get (op-table 'lookup))
(define put (op-table 'insert!))

(define true #t)
(define false #f)
(define display write)
(define display-string write-string)
(define newline (lambda () (write #\newline)))
(define (no-msg) (error "Message not found!"))
(define (display-line x)
  (begin
    (newline)
    (display x)))

;;; STREAM

(define stream-null? null?)
(define the-empty-stream '())

(define (stream-ref s n)
  (if (= n 0)
      (stream-car s)
      (stream-ref (stream-cdr s) (- n 1))))

(define (stream-map proc s)
  (if (stream-null? s)
      '()
      (cons-stream 
       (proc (stream-car s))
       $(stream-map proc (stream-cdr s)))))

(define (stream-for-each proc s)
  (if (stream-null? s)
      'done
      (begin 
        (proc (stream-car s))
        (stream-for-each proc 
                         (stream-cdr s)))))

(define (streams-map proc s1 s2)
  (if ((or (stream-null? s1) (stream-null? s2)))
      0
      (cons-stream
       (proc (stream-car s1) (stream-car s2))
       $(add-streams (stream-cdr s1) (stream-cdr s2)))))

(define (add-streams s1 s2)
  (streams-map + s1 s2))

(define (scale-stream s f)
  (stream-map
   (lambda (x) (* x f))
   s))

(define (display-stream s)
  (stream-for-each (lambda (x) (display x) (display " ")) s))

(define (cons-stream a b)
  (cons a (delay b)))

(define (stream-car stream)
  (car stream))

(define (stream-cdr stream)
  ((apply (cdr stream))))

(define (delay exp)
  (lambda () exp))

(define (force exp)
  (exp))

(define (stream-enumerate-interval low high)
  (if (> low high)
      '()
      (cons-stream
       low
       (stream-enumerate-interval (+ low 1)
                                  high))))
(define stream-range stream-enumerate-interval)

(define (stream-filter pred stream)
  (cond ((stream-null? stream) 
         '())
        ((pred (stream-car stream))
         (cons-stream 
          (stream-car stream)
          (stream-filter 
           pred
           (stream-cdr stream))))
        (else (stream-filter 
               pred 
               (stream-cdr stream)))))

(define (memo-proc proc)
  (let ((already-run? false) (result false))
    (lambda ()
      (if (not already-run?)
          (begin (set! result (proc))
                 (set! already-run? true)
                 result)
          result))))

(define (integers-starting-from n)
  (cons-stream
   n
   $(integers-starting-from (+ n 1))))

(define integers (integers-starting-from 1))

;;; ADDITIONAL PROGRAMS
(load "std/math.scm")
(load "std/set.scm")
