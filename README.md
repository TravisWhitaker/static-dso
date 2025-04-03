# static-dso

People say that you can't have more than one libc in the same program image.

## Probably Not Worth Solving

- Sharing anything non-trivial from the `libc` API.
    - We use our own primitives for everything anyway.
- Sharing `errno`
- Sharing TLS value keys
- Joining the client's `pthread`s and v-v.
- Locale madness.
- Don't share `FILE` handles.
    - They suck anyway.
- Freeing memory allocated by the client and v-v.
- Stack smashing protection.
    - Skill issue.
- Behavior differences.
    - `musl` gets it right, `glibc` is wrong, skill issue.

## Unclear If There's a Problem?

- Signal handlers?
    - Only the client should be installing these anyway.
    - `glibc` uses signals internally for `setuid` and thread cancellation.
    - `musl` doesn't so we should be ok?
- Environment variables?
    - Do we need to re-read `auxv` or something?
    - Your idea sucks if you need to do this anyway.
- `gdb` might say weird stuff.
- Client must be able to `dlopen`.
    - Lib also seems to be able to `dlopen`? Unclear if it's broken in a silent
      way.

## The Big Problem

Incompatible TCB/TLS layout between `musl` and `glibc`. It must be solved, not
even linker namespaces would save you from this.

## The Fix?

Maintain a set of lib `pthread` references in `musl` so we can tell which are
ours and which are the client's. Check this before each TLS access. If it's not
a TCB that we setup, fall back to a static global or tell the caller the
operation failed.

Good:
- Will never observe or interact with a TCB incorrectly.

Bad:
- TLS access is slower.
- Some operations become impossible.
    - Using TLS is a skill issue.
    - Graphics/GUI APIs will be called on the client side anyway.
