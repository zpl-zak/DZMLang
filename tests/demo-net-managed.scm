
(define server (make-socket))
(socket-listen server 1 7890) ;; sock, maxconn, port

;; accept 1 client
(define client (socket-accept server))

;; send him an object
(socket-write client '(1 2 3 4 "Hello World!" (list 1 2 3)))

;; receive a string from him
(define response (socket-read client))

;; print the response
(write "\nRESPONSE IS PAIR (DATA . SIZE):\n")
(write response)
(write "\nEND RESPONSE\n")

;; close sockets
(socket-close client)
(socket-close server)

;; demo used with: "ncat -e /bin/cat 127.0.0.1 7890" ..
