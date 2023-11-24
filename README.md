# templr

templr is a simple templating tool built using C.
It uses a csv file to take data as input, and a template file
which is used to template the output.

Example template:

```
To,
{name} (roll no. {rollno})
Student of BS-ES course, IIT Madras

Dear {name},

You scored {marks} marks in the exam, and your grade is {grade}.

Sincerely,
The Dean
```

Example csv:

```
Nitin C,abc123,50
Richard Feynman,xyz987,85
D Ritchie,iop567,99
```

the grade is inferred from the marks (3rd column) using the following
logic:

- marks >= 90: S
- 90 > marks >= 80: A
- 80 > marks >= 70: B
- 70 > marks >= 60: C
- 60 > marks >= 50: D
- 50 > marks >= 40: E
- marks < 40: U
