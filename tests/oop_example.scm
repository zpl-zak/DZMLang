(define (make-user username)
  ;; Type specifier
  (define (type-of) 'user)

  ;; Object scope
  (let ((table (make-table eq?))) ;; <- here, we define local members, they are implicitly private

    ;; These are our methods, again, they are implicitly private
    (define (set-password! p)
      (send 'update! table '(password) p))
    (define (set-level! l)
      (send 'update! table '(level) l))
    (define (set-name! n)
      (send 'update! table '(username) n))
    (define (print)
      (send 'print table))

    ;; Also called a dispatcher, returns a reference to our method, based on simple symbol lookup
    ;; This is a place where we map global symbols to local members, making them explicitly public
    (define (self m)
      (cond ((eq? m 'set-password!) set-password!)
            ((eq? m 'set-level!) set-level!)
            ((eq? m 'set-name!) set-name!)
            ((eq? m 'print) print)
            ((eq? m 'type-of) type-of)

            ;; We can't refer to an undefined method
            ;; This is also the place used by inherited object, which calls its super object to access base members.
            (else (no-msg))))

    ;; constructor, here we can modify our object
    (begin
      (set-name! username)

      ;; An object HAS to return a reference to self.
      ;; This reference has an access to living local environment defined by constructor.
      ;; This way, we can preserve data in a managed form, hence the name, object.
      ;; 'make-user' procedure could be called a class.
      self)))
