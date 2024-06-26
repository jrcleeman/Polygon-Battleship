#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

#include <vector>
#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib> 
#include <time.h> 

#include "Vec2.h"
//Shorthand for ptr
template <class T> using ptr = std::shared_ptr<T>;