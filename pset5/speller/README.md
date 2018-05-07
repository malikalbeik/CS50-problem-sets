# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

an invented long word said to mean a lung disease caused by inhaling very fine ash and sand dust.

## According to its man page, what does `getrusage` do?

returns information describing the resources utilized by the currentprocess, or all its terminated child pro-cesses.

## Per that same man page, how many members are in a variable of type `struct rusage`?

16

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

So that we don't copy the whole struct to calculate and thus wasting a lot of resources.

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file. In other words, convince us that you indeed understand how that function's `for` loop works.

First main opens the file and then reads the file character by character keeping a pointer to the current place which is called a stream. And when the stream reaches EOF, End Of File the loop stops and closes the file.

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?

fgetc reads the file character by character. fscanf on the other hand reads till white space and stores in a buffer, the buffer may contain commas dots or whatever else, which would cause problems with the spelling function.

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

because we don't want the function to be able to change the parameters.
