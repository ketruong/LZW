# LZW
This is an implementation of the Lempel-Ziv-Welch Algorithm which is a loseless
data compression algorithm. The program includes an encoder and a decoder.
This was written for CS323 @ Yale.  

## Encode 
1. Create the table 
2. Code = Empty, bits = 9
3. Send the number of bits to represent the maxbits and escape 
4. While (Char = getchar()) != EOF
    1. If the number of codes is greater that  2<sup>bits</sup> and less than 2<sup>maxbits</sup>
        1. bits = bits + 1
    2. If the (Code, Char) pair is in the hashtable 
        1. Set the code to be the index in the array 
    3. Else 
        1. If code == empty, this means that that the escape flag was specified
            1. Send escape code and Char
            2. If there is space in the table, add the (Code, Char)  pair in 
            3. Else prune the table 
        2. Else output the code
        3. If the (Char, Empty) pair is not in the table
            1. Go back a character and set code to be Empty 
        4. Else if there is space in the table
            1. Add the (Char, Code) pair in 
            2. Set the code to be the (Code, Empty) pair
        5. Else prune the table
5. If the code is not empty
    1. Send the code out

## Decode 

