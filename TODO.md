Project's TODO
------

Current tasks:
- Implement tokenize function that tokenize string by **string** delimiter and support last token highlighting. Means, if string "Hello, World" splitting by ", ", it will return ["Hello", "World"] and notify, that last token is termination token. Otherwise, if split "Hello, World!" by "!", it should return ["Hello, World"] and notify, that string is fully tokenized and there's no termination token. **Termination token** - is last token in string, if string ends without delimiter. If string ends with delimiter it can be called **fully tokenized**.
- Parse server messages with tokenization.
