https://github.com/user-attachments/assets/3a78cf3d-04d1-4479-8e49-9da09d3c7c48
# 🎨 3D Jigglypuff Animation - OpenGL Final Project

This repository contains the source code and documentation for our **Final Project** in the course **SECV2213 - Fundamental of Computer Graphics (Section 01)** at **Universiti Teknologi Malaysia (UTM)**.

## 👩‍💻 Project Title
**Interactive 3D Jigglypuff Model using OpenGL**

## 🧑‍🤝‍🧑 Team: TrioGraphiX
- **Nur Aina Balqis Binti Mohamad Zaparin** (A23CS0151)  
- **Kristine Elle Benjamin** (A23CS0095)  
- **Nur Amiera Zulaikha Binti Hardi** (A23CS0153)

## 📌 Overview
This project features an animated 3D model of **Jigglypuff** from Pokémon, built using **OpenGL** and **GLUT**. It builds upon our earlier submission in [Assignment 3](https://github.com/nurainabalqis/Jigglypuff-3D-Model-using-Hierarchical-Modelling), expanding the modular design, introducing complex animation, texture mapping, and audio interactivity.

### 🔧 Tools & Technologies
- C++ with OpenGL & GLUT
- BMP Texture Loader
- Windows Sound API
- Perspective & Orthographic Projections

## 🌟 Key Features
- 🎵 **Singing animation** with dynamic mouth movement and looping sound
- 👀 **Blinking eyes** using timer functions
- 🐝 **Bee swarm** animation orbiting around Jigglypuff
- 🕊️ **Flying simulation** with wing flapping
- 🖱️ **Mouse-controlled scene rotation**
- 💡 **Lighting & shading** for realism
- 🖼️ **Texture mapping** with BMP overlay image
- ⌨️ **Keyboard interactivity** for toggling animations and sound
- 📝 **On-screen text instructions**

## 🧱 Modeling Approach
The entire model is built using **GLUT primitives**, primarily `glutSolidSphere()`, combined with scaling, translation, and rotation to form body parts. Each component is organized into separate functions like `drawEye()`, `drawWing()`, and `drawMouth()`.

## 🏗️ Hierarchical Design
We used a hierarchical transformation structure via `glPushMatrix()` and `glPopMatrix()`, enabling parent-child relationships between components for smooth animation and interaction.


## ✅ Conclusion
This project demonstrates our understanding of key computer graphics principles including:
- 3D modeling
- Scene rendering
- Interaction design
- Animation
- Audio-visual integration

## 🧠 Credits
Supervised by: **Ts. Dr. Goh Eg Su**  
UTM Faculty of Computing - 2025


## 📚 References
- [LearnOpenGL](https://learnopengl.com)
- [GeeksforGeeks - OpenGL Animations](https://www.geeksforgeeks.org/dsa/creating-animations-using-transformations-opengl/)
- [BMP File Format - Wikipedia](https://en.wikipedia.org/wiki/BMP_file_format)
- [Jigglypuff | Pokémon Wiki](https://pokemon.fandom.com/wiki/Jigglypuff)
