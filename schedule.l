(de reset-schedule-queue ()
    (set 'schedule-queue Nil) )

(de nest (fn ticks)
   (if (<= ticks 0)
      fn
      (list Nil
	 (cons 'quote
	    (nest fn (- ticks 1)) ) ) ) )

(de schedule (lambda ticks)
   (push 'schedule-queue
      (cons ticks
	 lambda ) ) )

(de schedule-loop (lambda n)
   (schedule (list Nil
		(list (cons 'quote lambda))
	       (list 'schedule-loop (cons 'quote lambda) n))
	     n))

(de schedule-tick ()
    (set 'schedule-queue
	 (let (schedule-queue-old schedule-queue)
	   (let (schedule-queue Nil)
	     (let (schedule-queue-new
		   (mapcan
		    '((x) (if (<= (dec (car x)) 0)
			      (prog ((cdr x))
			       Nil)
			      (list (cons (dec (car x))
					  (cdr x)))))
		     schedule-queue-old ) )
	       (append schedule-queue schedule-queue-new) ) ) ) ) )

(de bang ()
    (schedule-tick))

(de schedule-demo ()
   (reset-schedule-queue)
   (schedule '(() (prinl 'monkey)) 3)
   (schedule '(()
	       (prinl 'badger)
	       (schedule '(() (prinl 'rat))
		  3 ))
      2 )
   schedule-queue )

(de flam (n speed lambda)
    (lambda)
    (for x (- n 1) (schedule lambda (* x speed))))

##(flam 16 1 '(() (pd-message 'hh)))


(de pat1 ()
    (flam 10 7 '(() (pd-message 'bd))))

(de pat2 ()
    (flam 4 4 '(() (pd-message 'hh))))

## (pat1)
## (pat2)

## (schedule-loop pat1 70)
## (schedule-loop pat2 16)
## (schedule-loop '(() (pd-message 'hh)) 4)

## (reset-schedule-queue)
(de hush ()
    (reset-schedule-queue))

(set 'rhyth '(hh - hh hh hh - - hh))

(de do-pattern (pat)
    (let (lambda Nil)
      (when (not (= (car pat) '-))
	(pd-message (car pat)))
      (for i (length (cdr pat))
	   (let (b (car (nth (cdr pat) i)))
	     (when (not (= b '-))
	       (schedule (list Nil
			    (list 'pd-message (cons 'quote b)))
			 i))))))

(de loop-pattern (pat)
    (schedule-loop (list Nil
			 (list 'do-pattern (cons 'quote pat)))
		   (length pat)))

## (do-pattern rhyth)
## (loop-pattern rhyth)

(prog
    (hush)
   (flam 8 1 '(() (pd-message 'hh)))
   (loop-pattern '(bd - - - bd - bd -))
   (loop-pattern '(hh - hh - hh)))

## (hush)
