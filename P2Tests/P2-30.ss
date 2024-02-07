(define (square n)
    (if (number? n)
        (* n n)
        'square_requires_a_numeric_argument
    )
)

(square)
