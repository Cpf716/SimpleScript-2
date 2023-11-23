# SimpleScript

## Statements

#### echo : any

#### consume : symbol

#### sleep : int

### Jump

#### assert : string

#### break

#### continue

#### throw : string

#### return : any

## Control Statements

### Function

#### func-end func : symbol

func foobar
    echo ""
end func

### Jump

#### try-catch-end try : symbol

try
    throw ""
catch err
end try

### Repetition

#### do while-end while

do while true
    continue
end while

#### for-end for

for ,,
    continue
end for

#### while-end while

while true
    continue
end while

### Selection

#### if-end if

if true
    echo ""
end if
