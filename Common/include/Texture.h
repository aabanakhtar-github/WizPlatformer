#ifndef TEXTURE_H
#define TEXTURE_H

#include <string> 
#include <cstdint> 
#include <unordered_map>
#include "SDL.h"
#include "Util.h"

class Window;

class TextureData {
public:
	TextureData();
	TextureData(const TextureData&);
	TextureData(TextureData&&) noexcept;
	TextureData& operator = (TextureData);
	TextureData& operator = (TextureData&&) noexcept;
	~TextureData();
	
	bool load(Window& window, const std::string& filepath);
    // TODO: never tested
    bool refresh();
	// cool feature
	friend void swap(TextureData& a, TextureData& b) {
		using std::swap;
		swap(a.Surface, b.Surface);
		swap(a.Texture, b.Texture);
		swap(a.m_Width, b.m_Width);
		swap(a.m_Height, b.m_Height);
		swap(a.m_Renderer, b.m_Renderer);
		swap(a.m_Valid, b.m_Valid);
	}

	[[nodiscard]] int GetWidth() const { return  m_Width; }
	[[nodiscard]] int GetHeight() const { return m_Height; }
	[[nodiscard]] bool GetValid() const { return m_Valid; }
public:
	SDL_Surface* Surface; 
	SDL_Texture* Texture;

private:
	int m_Width;
	int m_Height;
	SDL_Renderer* m_Renderer;
	bool m_Valid;

};

class TextureManager
{
	MAKE_SINGLETON(TextureManager)

public:
	TextureManager() = default; 

	TextureData& GetTexture(const std::string& key)	{
        return m_Textures[key];
	}

	bool AddTexture(Window& window, const std::string& filepath, const std::string& key)
	{
		TextureData t;
		if(!t.load(window, filepath))
		{
			return false;
		}

		m_Textures.insert({ key, std::move(t) });
		return true;
	}
private:
	std::unordered_map<std::string, TextureData> m_Textures;
};

#endif