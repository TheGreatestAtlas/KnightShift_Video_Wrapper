# KnightShift Video Wrapper

# PL:

## Opis
Ten program umożliwia zastąpienie oryginalnych plików wideo intro w grze **KnightShift** dowolnym formatem wideo.  
Zamiast plików `.twv` uruchamiane są materiały wideo przez **ffplay.exe** w trybie pełnoekranowym.  
Rozwiązanie działa wyłącznie dla filmów intro odtwarzanych przy starcie gry.

## Jak to działa
1. Program wywołuje obrazek `SPG.wd1`.  
2. Odtwarza wszystkie pliki wideo znajdujące się w katalogu `Video` przy użyciu `ffplay.exe`.  
3. Po zakończeniu uruchamia główny plik `.exe` gry.  

## Wymagania
- `ffplay.exe` (część pakietu [FFmpeg](https://ffmpeg.org/))  
- Zainstalowana gra **KnightShift**  

## Instalacja i użycie
1. Skopiuj pliki wrappera do katalogu gry.  
2. Upewnij się, że w folderze `Video` znajdują się pliki wideo, które chcesz odtworzyć.  
3. Uruchom wrapper zamiast oryginalnego `.exe` gry.  

---

# EN:

## Description
This program allows you to replace the original intro video files in the game **KnightShift** with any video format.  
Instead of `.twv` files, videos are played by **ffplay.exe** in full-screen mode.  
This solution only works for intro videos played at the start of the game.

## How it works
1. The program calls the image `SPG.wd1`.  
2. It plays all video files located in the `Video` directory using `ffplay.exe`.  
3. When finished, it launches the main `.exe` file of the game.  

## Requirements
- `ffplay.exe` (part of the [FFmpeg](https://ffmpeg.org/) package)  
- **KnightShift** game installed  

## Installation and use
1. Copy the wrapper files to the game directory.  
2. Make sure that the `Video` folder contains the video files you want to play.  
3. Run the wrapper instead of the original game `.exe`.  

