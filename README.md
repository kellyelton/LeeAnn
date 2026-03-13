# LeeAnn

A from-scratch C++ neural network library that arranges neurons in a 3D spatial grid ("NeuroCube") with neighbor connectivity, sigmoid activation, and reward-based learning. Includes a graphical demo (Allegro 5) where a neural-network-controlled creature chases a bouncing food target on screen.

## How It Works

1. A `NeuroCube` is constructed with X/Y/Z dimensions (e.g., 3x3x3 or 5x5x5), creating a grid of `Neuron` objects at each spatial position
2. Neurons are automatically connected to their spatial neighbors — within each layer (right, bottom-right, bottom) and across layers to the next depth (all 9 surrounding positions)
3. Dedicated input and output neurons are attached at specified grid coordinates, bridging external data to the internal cube
4. When input is fed, each neuron collects incoming data, then a pluggable `Algorithm` computes outputs using a sigmoid activation function: `1 / (1 + e^(-activation * weight))`
5. Signals propagate through the cube via `Push()` — each neuron fires its weighted outputs to connected neighbors
6. Output neurons trigger an `OutputEventCatcher` callback when they receive data
7. Learning happens through `Reward()` — positive reward increases the weights of the highest-weighted output connections and decreases the rest, propagating backward through the network. This is reward-based weight adjustment, not backpropagation

## Architecture

### Core Library (`LeeAnn/`)

| Component | Description |
|---|---|
| **`Neuron`** | The fundamental unit. Maintains input/output connections (as `Position` references into the cube), output weights, input event history, and a reward timestamp. Supports dedicated I/O mode via `OutputEventCatcher`. |
| **`NeuroCube`** | The 3D grid container. Creates neurons at each (x,y,z) position, wires neighbor connections via `MakeConnections()`, and orchestrates the update cycle across dedicated inputs → internal neurons → dedicated outputs. |
| **`Algorithm`** | Abstract base class with a virtual `CalculateOutputs()` method. Implementations define how input data is transformed into output signals. The provided `Basic` algorithm sums inputs and applies a sigmoid per output connection. |
| **`Position`** | Simple (X,Y,Z) struct used for spatial addressing within the cube. Dedicated I/O neurons use negative coordinates to distinguish them from internal neurons. |
| **`InputEvent`** | Records each input with its data, output weights, output values, and timestamp — used by the reward system to adjust weights retroactively. |
| **`OutputEventCatcher`** | Virtual callback interface. When an output neuron receives data, it fires `onOutput()` to notify the host application. |
| **`NeuronMath`** | Static utility class: `RandFloat()`, `RandomClamped()` (returns -1 to 1), and `dtos()` (double-to-string). |
| **`nArray<T>`** | Custom dynamic array template under `Skylabs::Common` namespace with geometric growth (doubles capacity on overflow). Not used in the final architecture — appears to be an earlier experiment replaced by `std::deque`. |

### Experimental Prototype (`baby.h`)

| Component | Description |
|---|---|
| **`Baby`** | Simplified neuron concept with only input connections and a `Tickle()` propagation method. No weights, no learning — just connectivity testing. |
| **`BabyLayer`** | 2D grid of `Baby` nodes with neighbor connections (right, bottom-right, bottom). |
| **`BabyCube`** | 3D stack of `BabyLayer`s with cross-layer connections. Appears to be a prototype for validating the NeuroCube connectivity logic before adding weights and learning. |

### Graphical Demo (`demo1/`)

A visual simulation using **Allegro 5** where a `Creature` with a NeuroCube "brain" (5x5x5) chases a bouncing food target:

- Two dedicated inputs feed the creature's position and the food's position into the network
- Two dedicated outputs control left/right steering forces (tank-drive model: differential thrust → rotation + speed)
- When the creature overlaps the food, it receives a small reward (0.005) that propagates through the network
- The creature, food, and neural output values are rendered at 60 FPS with a TrueType font overlay

## Tech Stack

- **Language**: C++ (Visual Studio 2010, later upgraded to VS 2012 toolset v110)
- **Dependencies**: Boost 1.46.1 → 1.54.0 (shared_ptr), Allegro 5.0.9 (demo only)
- **Platform**: Win32
- **Build**: Visual Studio solution with two projects — `LeeAnn` (static library) and `demo1` (console/graphical application linking against LeeAnn)

## Development Timeline

The solution file targets **Visual Studio 2010** format, and the original build referenced **Boost 1.46.1** (released March 2011), placing earliest possible development at spring 2011. The project was pushed to GitHub on **July 26, 2012** in a single commit ("Added files"), suggesting it was developed offline and uploaded retroactively.

A later session on **July 18, 2013** added explicit `#include <boost\shared_ptr.hpp>` to Neuron.h and Neuron.cpp, upgraded the project to the VS 2012 toolset (v110), and updated Boost paths to 1.54.0. The project was briefly reopened in VS 2017 (June 2017) and VS 2019 (July 2020) but no source changes were made in those sessions.

Over **3 commits across ~1 year** (July 2012 – July 2013):
- **Jul 26, 2012**: Initial push — full library (Neuron, NeuroCube, Algorithm, NeuronMath, nArray), the Baby prototype, and the Allegro 5 graphical demo. All committed in one shot.
- **Jul 18, 2013**: Boost compatibility fix — added explicit boost shared_ptr includes, upgraded to VS 2012 toolset and Boost 1.54.0.

## AI Code Review

*Code review performed by AI (Claude) and graded relative to the era the code was written in.*

| Criterion | Grade | Notes |
|---|---|---|
| **Completeness** | C+ | Core neural network library works — neurons, connections, sigmoid activation, reward propagation, pluggable algorithm. But training doesn't converge, no backpropagation, no serialization. Demo runs but the creature doesn't meaningfully learn. |
| **Functionality** | C | Code compiles and runs but has several bugs: `Neuron * n = &Neuron()` creates a dangling pointer to a temporary, `shared_ptr` wraps stack objects (undefined behavior), `XYZtoIndex` formula appears incorrect for non-cubic grids, reward propagation can index out of bounds. The creature moves but learning is non-functional. |
| **Patterns & Practices** | C | For 2012 C++: virtual base class for Algorithm (good), `#pragma once` (good), but `using namespace std` in headers, raw pointer/shared_ptr confusion, mixing ownership models. Pre-C++11 era makes some of this more forgivable. |
| **Code Quality** | C+ | Reasonably organized across files, decent naming (NeuroCube, Neuron, Position, Algorithm). Some rough spots: commented-out debug code throughout, `int illshit` variable name, inconsistent formatting. But the intent is clear throughout. |
| **Architecture** | B- | The 3D NeuroCube concept is genuinely interesting — neurons arranged spatially with neighbor connections across layers. Dedicated I/O neuron abstraction, pluggable Algorithm pattern, event-based output catching. The architecture shows real thought about how to structure a neural network differently from textbook implementations. |
| **Ambition** | A- | Building a from-scratch neural network in C++ with a 3D spatial topology, reward-based learning, and a graphical demo with Allegro — as a hobby project before the deep learning revolution — is very ambitious. The NeuroCube concept of spatially-arranged neurons with neighbor connectivity is a creative architectural approach. |
| **Time Investment** | B | The codebase is ~800 lines of meaningful code across a library plus a graphical demo, committed in a single push. For an estimated few sessions of work, this is solid output. |

**Overall: B-** — An ambitious, creative project that was ahead of its time conceptually. The 3D NeuroCube architecture with spatial neighbor connectivity and reward-based learning shows genuine original thinking about neural network topology. The code has real bugs (dangling pointers, ownership confusion) and the learning mechanism doesn't converge, but for a hobby project written before the deep learning revolution, it demonstrates impressive initiative and ambition. The Allegro demo showing a neural-network-controlled creature is a nice touch that shows the developer wanted to visualize and validate the concept, not just write abstract math.

## Idea Timing

LeeAnn was committed to GitHub two months before **AlexNet** (September 2012), the event universally cited as the start of the modern deep learning era. It predates every major deep learning framework.

| Date | Event |
|------|-------|
| 1988 | Cellular Neural Networks (Chua & Yang) — neurons on a grid with local neighbor connectivity, the closest structural ancestor to NeuroCube |
| 1992 | Polyworld (Yaeger) — neural-network-controlled creatures foraging in a 2D world, the closest conceptual ancestor to demo1 |
| 1994 | Karl Sims "Evolved Virtual Creatures" — NN-controlled 3D creatures, ran on supercomputers |
| 2002 | NEAT (Stanley & Miikkulainen) — neuroevolution of augmenting topologies |
| 2003 | FANN — Fast Artificial Neural Network Library in C, feedforward only |
| 2005 | NERO game — real-time neuroevolution controlling virtual soldiers |
| 2007 | HyperNEAT — spatial geometry determines neural connectivity via CPPNs |
| 2007 | Reward-modulated Hebbian learning papers (Izhikevich, Florian) — synaptic changes gated by reward signals, similar to LeeAnn's approach |
| 2008 | Theano development begins (Python, academic) |
| **2011–2012** | **LeeAnn — 3D NeuroCube with reward-based learning and graphical agent simulation** |
| 2012 (Sep) | AlexNet wins ImageNet — the "Big Bang" of deep learning |
| 2013 (Dec) | Caffe released; DeepMind DQN paper ("Playing Atari with Deep RL") |
| 2015 | TensorFlow, Keras, MarI/O (NEAT plays Mario — popularizes neuroevolution to mainstream) |
| 2016 | PyTorch, AlphaGo, OpenAI Gym |
| 2023 | Spatially Embedded RNNs (Nature Machine Intelligence) — the modern academic formalization of neurons positioned in 3D Euclidean space with distance-dependent connectivity |

The NeuroCube's 3D spatial topology was genuinely unconventional — not a standard layered network, not NEAT, not a self-organizing map. It has more in common with Cellular Neural Networks (1988) and modern spatially-embedded RNNs (2023) than with anything mainstream in 2012. The reward-based learning predates the deep RL revolution (DQN, December 2013) by ~18 months. The neural-network-controlled creature fits the artificial life tradition (Polyworld, Karl Sims, NEAT) but was built from scratch without using any existing framework, at a time when the deep learning ecosystem simply did not exist.

**Grade: A-** — The developer was exploring neural networks before the deep learning era began, using a spatial topology that wouldn't receive formal academic treatment for another decade, and applying reward-based learning before deep RL went mainstream. For a hobbyist working in the framework vacuum of 2011-2012, this was a distinctly forward-looking project.
