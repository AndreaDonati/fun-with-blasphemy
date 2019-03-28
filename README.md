# fun-with-blasphemy
A simple, yet fun, executable+script utility to harras your friends with randomly-generated blasphemy.

## How to use
First of all, you have to have the "telegram-cli" package installed on your Linux system.
If you already have it, just go to the "utils" folder and run the script in the terminal with the following syntax:


```./script.sh [name of your telegram contact]```

Please note that, if your contact's name has a space in it, you have to replace it with an underscore.

Have fun.

## Updating the Dictionary

If you want to change the content of the dictionary to add/remove some words, be sure to update the first line of the file with the new number of rows contained in the file. Let your mind run free and add as many words as you want.

If you have some funny suggestions, feel free to open an issue or send a pull request.

## Some More Info

The "input.txt" file in the "utils" folder contains the input to give to the blasphemy-generator. It's syntax is the following

```First Row: number of productions (i.e. number of plasphemies to generate)```  
```Second Row: number of maximum elements that will compose the blasphemy (basically, it's maximum lenght)```
