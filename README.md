# vim, ed, shell, whatever

## Build

For reasonable, logical and intuitive use cases:

```console
$ make
```

For teacher assistants:

```console
$ make CDEFINES=-DIDIOT
```

## Run

Legacy mode (phase 1):

```console
$ LEGACY=1 ./main [FILE]
```

Normal mode (phase 2):

```console
$ ./main [FILE]
```
