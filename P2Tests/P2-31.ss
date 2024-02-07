(define (cond_ex_2 param)
        (cond ((= param 1) 'The_value_is_1)
              ((= param 2) 'The_value_is_2)
              ((= param 12) 'The_value_is_12)
        )
)

(cond_ex_2 1)