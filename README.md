# About 

This web-based tool is designed to inspect and interact Amiga media files such as ADF and HDF
disk images. It brings the classic Amiga command-line experience directly to your browser, no
installation required. Getting started is simple: Just drag and drop an ADF or HDF file onto the
browser window to launch a fully functional command shell. Once inside the shell, you can
navigate directories and view file contents, create and delete files and folders, or run
built-in file system diagnostics. It's an intuitive and efficient way to explore the inner
structure of your Amiga disk images.

## Technical Background
	
Behind the scenes, a modern web-frontend implemented in Svelte interacts with the Amiga emulator
*vAmiga* compiled to WebAssembly (WASM). This setup allows it to emulate the Amiga file system
and command interface entirely in the browser, delivering fast performance and seamless usability
across platforms.

## Deployment

[https://vamigados.github.io](https://vamigados.github.io)

## Developers Corner

```bash
# To build the project locally...

# Clone the project
git clone https://github.com/vAmigaDOS/vAmigaDOS.git

# Install components
cd vAmigaDOS 
npm install

# Setup a build directory
mkdir build
emcmake cmake -S cpp -B build

# Build the WASM part
cd build
make -j

# Option 1: Run a local build
cd ..
npm run dev

# Option 2: Deploy
npm run build
```
