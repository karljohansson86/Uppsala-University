vodulus = string
severian = string -> integer 


severian "10" =
    if "10" == ""
        then 0
        else severian (take (length "10" - 1) "10") * 2 +
            if drop (length "10" - 1) "10" == "0"
                then 0
                else 1


severian "10" =
    False 
        then 0
        else severian (take (length "10" - 1) "10") * 2 +
            if drop (length "10" - 1) "10" == "0"
                then 0
                else 1

severian "10" =
        severian (take (2-1) "10") * 2 +
            if drop (length "10" - 1) "10" == "0"
                then 0
                else 1

severian "10" =
        severian (take (1) "10") * 2 +
            if drop (length "10" - 1) "10" == "0"
                then 0
                else 1

severian "10" =
        (severian ("1") * 2 +
            if drop (length "10" - 1) "10" == "0"
                then 0
                else 1





severian "1" =
	if "1" == ""
        then 0
        else severian (take (length "1" - 1) "1") * 2 +
            if drop (length "1" - 1) "1" == "0"
                then 0
                else 1


severian "1" =
    False
        else severian (take (length "1" - 1) "1") * 2 +
            if drop (length "1" - 1) "1" == "0"
                then 0
                else 1

severian "1" =
	severian (take (1-1) "1") * 2 +
            if drop (length "1" - 1) "1" == "0"
                then 0
                else 1


severian "1" =
	severian (take (0) "1") * 2 +
            if drop (length "1" - 1) "1" == "0"
                then 0
                else 1


severian "1" =
	severian ("") * 2 +
            if drop (length "1" - 1) "1" == "0"
                then 0
                else 1



severian "" =
    if "" == ""
        then 0
        else severian (take (length "" - 1) "") * 2 +
            if drop (length "" - 1) "" == "0"
                then 0
                else 1

severian "" =
    True
        then 0

severian "" = 0
        


återgår till severian "1" med värdet 0 på severian ("")
severian "1" =
    severian ("") * 2 +
            if drop (length "1" - 1) "1" == "0"
                then 0
                else 1
                
severian "1" =
		0 * 2 +
            if drop (length "1" - 1) "1" == "0"
                then 0
                else 1


severian "1" =
		0 +
            if drop (length "1" - 1) "1" == "0"
                then 0
                else 1


severian "1" =
		0 +
            if drop (1 - 1) "1" == "0"
                then 0
                else 1

severian "1" =
		0 +
            if drop (0) "1" == "0"
                then 0
                else 1

severian "1" =
		0 +
            if "1" == "0"
                then 0
                else 1

severian "1" =
		0 +
            False
                then 0
                else 1

severian "1" =
		0 +
            False
                else 1

severian "1" =
		0 + 1

severian "1" = 1



återgår till severian "10" med värdet 1 på severian ("1")
severian "10" =
        severian ("1") * 2 +
            if drop (length "10" - 1) "10" == "0"
                then 0
                else 1

severian "10" =
        1 * 2 +
            if drop (length "10" - 1) "10" == "0"
                then 0
                else 1

severian "10" =
        2 +
            if drop (length "10" - 1) "10" == "0"
                then 0
                else 1

severian "10" =
        2 +
            if drop (2 - 1) "10" == "0"
                then 0
                else 1

severian "10" =
        2 +
            if drop (1) "10" == "0"
                then 0
                else 1

severian "10" =
        2 +
            if "0" == "0"
                then 0
                else 1

severian "10" =
        2 +
            True
                then 0
                else 1

severian "10" =
        2 +
            True
                then 0
                

severian "10" =
        2 + 0
                
severian "10" = 2

