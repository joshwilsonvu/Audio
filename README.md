# Audio
The Unix precursor to AudioScript that reads audio files, processes them, and writes them back to disk.

## Build
From the `bin` directory, run
```
    $ cmake ..
    $ make
```
## Usage

1. From the `bin` directory, run
```
    $ ./Audio
```
2. Enter the filename of the audio file to process (any kind if the `sox` binary runs, `.wav` files only if not).

3. Enter a series of processing effects to apply as directed.

4. Enter the word "Process."

5. Use the file `<filename>_processed.<extension>`.

## Extensions

To add a new processing effect `MyEffect`, do the following:

1. Create the files `include/ProcessCommands/MyEffect.h` and `src/ProcessCommands/MyEffect.cpp`.

2. Subclass the `ProcessCommand` abstract class, overriding the `virtual void execute (sample_t* first, sample_t* last, sample_t* result)`
method, forward-declaring the `Factory` class, and adding the `Factory` class as a `friend`.

3. In `include/Factory.h`, declare one or more `makeMyEffect(params...)` methods.

4. In `src/Factory.cpp`, implement the `makeMyEffect()` methods, using `sampleRate()` and `numChannels()` if desired.

5. In `include/Interpreter.h`, add an entry `MyEffect` to the Interpreter::PCType enum.

6. In `src/Interpreter.cpp`, in `Interpreter::Interpreter()`, add the line `pcMap["myeffect"] = PCType::MyEffect;`.

7. In `src/Interpreter.cpp`, in `Interpreter::readProcessCommandImpl()`, add a switch case that parses the vector 
of input tokens with `getDouble()` and calls the appropriate `factory.makeMyEffect()` method and returns its result.

## See also

My repository AudioScript is in development to be a sophicsticated and easily extensible real time audio processor. 
It will provide its own IDE, compile code on the fly, and integrate it as a plugin, with a graphical interface 
and the ability to change parameters mid-processing.
