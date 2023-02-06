#pragma once
#include <memory>
#include <D3D11.h>
#include <d3d9.h>
#include <D2D1.h>
#include <unordered_map>
#include <dwrite_1.h>
#include <string_view>
#pragma comment( lib, "dxgi" )
#pragma comment( lib, "d2d1" )
#pragma comment( lib, "dcomp" )
#pragma comment( lib, "dwrite" )

#define M_PI 3.14159265358979323846f


#define RET_CHK(x) if ( x != S_OK ) return

namespace Renderer {
	ID2D1Factory* m_pInterface;
	ID2D1RenderTarget* m_pCanvas;
	IDWriteFactory1* m_pTextEngine;
	IDWriteTextFormat* m_pTextFormat;
	ID2D1SolidColorBrush* m_pSolidBrush;
	bool initialized = false;

	bool Init(IDXGISwapChain* SwapChain) {
		if (!initialized) {
			initialized = true;
			D2D1_FACTORY_OPTIONS CreateOpt = { D2D1_DEBUG_LEVEL_NONE };
			DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pTextEngine), (IUnknown**)&m_pTextEngine);
			D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &CreateOpt, (void**)&m_pInterface);
			m_pTextEngine->CreateTextFormat(wxorstr_(L"Verdana"), NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 9.f, L"", &m_pTextFormat);
			if (!m_pInterface || !m_pTextEngine || !m_pTextFormat) return false;
		}

		ID3D11Device* d3d_device;
		if (SwapChain->GetDevice(IID_PPV_ARGS(&d3d_device))) 
			return false;

		WORD flagsOffset = *(WORD*)((*(uintptr_t**)d3d_device)[ 38 ] + 2);
		int& flags = *(INT*)((uintptr_t)d3d_device + flagsOffset);
		d3d_device->Release( );

		IDXGISurface* d3d_bbuf;
		if (SwapChain->GetBuffer(0, IID_PPV_ARGS(&d3d_bbuf)))
			return false;

		D2D1_RENDER_TARGET_PROPERTIES d2d_prop = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

		// set flags just for target creating,
		flags |= 0x20;
		HRESULT canvas_state = m_pInterface->CreateDxgiSurfaceRenderTarget(d3d_bbuf, d2d_prop, &m_pCanvas); 
		flags &= ~0x20;

		d3d_bbuf->Release( );

		if (canvas_state)
			return false;

		if (!m_pSolidBrush)
			m_pCanvas->CreateSolidColorBrush({}, &m_pSolidBrush);

		return true;
	}

	bool new_frame(IDXGISwapChain* SwapChain) {
		if (!m_pCanvas && !Init(SwapChain))
			return false;

		m_pCanvas->BeginDraw( );
		return true;
	}

	void reset_canvas( ) {
		if (m_pCanvas) {
			m_pCanvas->Release( );
			m_pCanvas = nullptr;
		}
	}

	void rectangle_filled(Vector2 pos, Vector2 size, const Color3 color) {
		m_pSolidBrush->SetColor(D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
		m_pCanvas->FillRectangle(D2D1::RectF(pos.x, pos.y, pos.x + size.x, pos.y + size.y), m_pSolidBrush);
	}

	void rectangle_filled2(Vector2 pos, const Color3 color) {
		m_pSolidBrush->SetColor(D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
		m_pCanvas->FillRectangle(D2D1::RectF(pos.x, pos.y, pos.x , pos.y), m_pSolidBrush);
	}

	void rectangle(Vector2 pos, Vector2 size, const Color3 color, float thickness = 1.2f) {
		m_pSolidBrush->SetColor(D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
		m_pCanvas->DrawRectangle(D2D1::RectF(pos.x, pos.y, pos.x + size.x, pos.y + size.y), m_pSolidBrush, thickness, nullptr);
	}
	void Rectangle(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& Clr, float Thick = 1.5f) {
		m_pSolidBrush->SetColor(Clr);
		m_pCanvas->DrawRectangle({ Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y }, m_pSolidBrush, Thick);
	}
	__forceinline void FillRectangle(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& Clr) {
		m_pSolidBrush->SetColor(Clr); m_pCanvas->FillRectangle({ Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y }, m_pSolidBrush);
	}
	void rectangle_filled(Square2 rec, const Color3 color) {
		m_pSolidBrush->SetColor(D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
		m_pCanvas->FillRectangle(D2D1::RectF(rec.pos.x, rec.pos.y, rec.pos.x + rec.size.x, rec.pos.y + rec.size.y), m_pSolidBrush);
	}

	void rectangle(Square2 rec, const Color3 color, float thickness = 1.2f) {
		m_pSolidBrush->SetColor(D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
		m_pCanvas->DrawRectangle(D2D1::RectF(rec.pos.x, rec.pos.y, rec.pos.x + rec.size.x, rec.pos.y + rec.size.y), m_pSolidBrush, thickness, nullptr);
	}

	void line(const Vector2 start, const Vector2 end, Color3 color, bool outl = false, float thickness = 1.f) {
		if (outl) {
			m_pSolidBrush->SetColor(D2D1::ColorF(D3DCOLOR_RGBA(0, 0, 0, 255), 1.f));
			m_pCanvas->DrawLine({ start.x, start.y }, { end.x, end.y }, m_pSolidBrush, thickness * 2);
		}
		m_pSolidBrush->SetColor(D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
		m_pCanvas->DrawLine({ start.x, start.y }, { end.x, end.y }, m_pSolidBrush, thickness);
	}
	void Line2(const Vector2& Start, const Vector2& End, const D2D1::ColorF& Clr, float Thick = 1.5f, bool outline = false) {
		if (outline) {
			m_pSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			m_pCanvas->DrawLine({ Start.x, Start.y }, { End.x, End.y }, m_pSolidBrush, Thick * 1.75);
		}
		m_pSolidBrush->SetColor(Clr);
		m_pCanvas->DrawLine({ Start.x, Start.y }, { End.x, End.y }, m_pSolidBrush, Thick);
	}
	void Triangle(const Vector2 top, const Color3 clr, float sz) {
		line(top, top + Vector2(sz, sz), clr);
		line(top, top - Vector2(sz, -sz), clr);
		line(top + Vector2(sz, sz), top - Vector2(sz, -sz), clr);
	}
	void circle(const Vector2 start, Color3 color, float radius, float thickness) {
		m_pSolidBrush->SetColor(D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
		m_pCanvas->DrawEllipse({ { start.x, start.y }, radius, radius }, m_pSolidBrush, thickness);
	}

	Vector2 get_text_size(std::wstring text, float sz) 	{
		const auto str = text;
		const auto str_len = static_cast<std::uint32_t>(str.length( ));

		IDWriteTextLayout* dwrite_layout = nullptr;
		RET_CHK(m_pTextEngine->CreateTextLayout(str.c_str(), str_len, m_pTextFormat, screen_size.x, screen_size.y, &dwrite_layout)) Vector2(0,0);

		const DWRITE_TEXT_RANGE range
		{
			0,
			str_len
		};

		dwrite_layout->SetFontSize(sz, range);

		DWRITE_TEXT_METRICS metrics;
		dwrite_layout->GetMetrics(&metrics);

		return Vector2(metrics.width, metrics.height);
	}


	void gradient_rect(Vector2 pos, Vector2 size, const Color3 color, const Color3 color_2, bool horizontal) {
		D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES linearGradientBrushProperties = {};
		ID2D1GradientStopCollection* pGradientStops = NULL;
		ID2D1LinearGradientBrush* m_pLinearGradientBrush;

		D2D1_GRADIENT_STOP gradientStops[ 2 ];
		gradientStops[ 0 ].color = D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
		gradientStops[ 0 ].position = 0.0f;
		gradientStops[ 1 ].color = D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
		gradientStops[ 1 ].position = 1.0f;

		RET_CHK(m_pCanvas->CreateGradientStopCollection(
			gradientStops,
			2,
			D2D1_GAMMA_2_2,
			D2D1_EXTEND_MODE_CLAMP,
			&pGradientStops
		));

		RET_CHK(m_pCanvas->CreateLinearGradientBrush(
			D2D1::LinearGradientBrushProperties(
				D2D1::Point2F(pos.x, pos.y),
				D2D1::Point2F(pos.x + size.x, horizontal ? pos.y : pos.y + size.y)
			),
			pGradientStops,
			&m_pLinearGradientBrush
		));

		m_pCanvas->FillRectangle(D2D1::RectF(pos.x, pos.y, pos.x + size.x, pos.y + size.y), m_pLinearGradientBrush);
	}

	void filled_circle(const Vector2 start, Color3 color, float radius) {
		m_pSolidBrush->SetColor(D2D1::ColorF(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
		m_pCanvas->FillEllipse({ { start.x, start.y}, radius,radius }, m_pSolidBrush);
	}

	template <typename ...Args>
	void text2(const Vector2 pos, const D2D1::ColorF& clr, const float sz, bool center, bool outline, const std::wstring_view text, Args&&... args) {
		const auto size = static_cast<std::size_t>(std::swprintf(nullptr, 0, text.data(), std::forward<Args>(args)...) + 1);

		const std::unique_ptr<wchar_t[]> buffer(new wchar_t[size]);
		std::swprintf(buffer.get(), size, text.data(), std::forward<Args>(args)...);

		const auto str = std::wstring(buffer.get(), buffer.get() + size - 1);
		const auto str_len = static_cast<std::uint32_t>(str.size());

		IDWriteTextLayout* dwrite_layout = nullptr;
		RET_CHK(m_pTextEngine->CreateTextLayout(str.c_str(), str_len, m_pTextFormat, screen_size.x, screen_size.y, &dwrite_layout));

		const DWRITE_TEXT_RANGE range
		{
			0,
			str_len
		};

		dwrite_layout->SetFontSize(sz, range);

		if (center) {
			m_pSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			DWRITE_TEXT_METRICS TextInfo;
			dwrite_layout->GetMetrics(&TextInfo);
			Vector2 TextSize = { TextInfo.width / 2.f, TextInfo.height / 2.f };
			const auto x = pos.x - TextSize.x;
			const auto y = pos.y - TextSize.y;
			if (outline) {

				m_pCanvas->DrawTextLayout(D2D1::Point2F(x - 1, y), dwrite_layout, m_pSolidBrush);
				m_pCanvas->DrawTextLayout(D2D1::Point2F(x + 1, y), dwrite_layout, m_pSolidBrush);
				m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y - 1), dwrite_layout, m_pSolidBrush);
				m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y + 1), dwrite_layout, m_pSolidBrush);
			}

			m_pSolidBrush->SetColor(D2D1::ColorF(clr.r / 255.f, clr.g / 255.f, clr.b / 255.f, clr.a / 255.f));

			m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y), dwrite_layout, m_pSolidBrush);
			dwrite_layout->Release();
			return;
		}


		m_pSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));

		if (outline) {
			const auto x = pos.x;
			const auto y = pos.y;

			m_pCanvas->DrawTextLayout(D2D1::Point2F(x - 1, y), dwrite_layout, m_pSolidBrush);
			m_pCanvas->DrawTextLayout(D2D1::Point2F(x + 1, y), dwrite_layout, m_pSolidBrush);
			m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y - 1), dwrite_layout, m_pSolidBrush);
			m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y + 1), dwrite_layout, m_pSolidBrush);
		}

		m_pSolidBrush->SetColor(D2D1::ColorF(clr.r / 255.f, clr.g / 255.f, clr.b / 255.f, clr.a / 255.f));
		m_pCanvas->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), dwrite_layout, m_pSolidBrush);
		dwrite_layout->Release();
	}
	template <typename ...Args>
	void t(const Vector2 pos, const Color3 clr, const float sz, bool center, bool outline, const std::wstring_view text, Args&&... args) {
		const auto size = static_cast<std::size_t>(std::swprintf(nullptr, 0, text.data( ), std::forward<Args>(args)...) + 1);

		const std::unique_ptr<wchar_t [ ]> buffer(new wchar_t[ size ]);
		std::swprintf(buffer.get( ), size, text.data( ), std::forward<Args>(args)...);

		const auto str = std::wstring(buffer.get( ), buffer.get( ) + size - 1);
		const auto str_len = static_cast<std::uint32_t>(str.size( ));

		IDWriteTextLayout* dwrite_layout = nullptr;
		RET_CHK(m_pTextEngine->CreateTextLayout(str.c_str( ), str_len, m_pTextFormat, screen_size.x, screen_size.y, &dwrite_layout));

		const DWRITE_TEXT_RANGE range
		{
			0,
			str_len
		};

		dwrite_layout->SetFontSize(sz, range);

		if (center) {
			m_pSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			DWRITE_TEXT_METRICS TextInfo;
			dwrite_layout->GetMetrics(&TextInfo);
			Vector2 TextSize = { TextInfo.width / 2.f, TextInfo.height / 2.f };
			const auto x = pos.x - TextSize.x;
			const auto y = pos.y - TextSize.y;
			if (outline) {

				m_pCanvas->DrawTextLayout(D2D1::Point2F(x - 1, y), dwrite_layout, m_pSolidBrush);
				m_pCanvas->DrawTextLayout(D2D1::Point2F(x + 1, y), dwrite_layout, m_pSolidBrush);
				m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y - 1), dwrite_layout, m_pSolidBrush);
				m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y + 1), dwrite_layout, m_pSolidBrush);
			}
			//Renderer::rectangle_filled({ x, y }, { sz * 11, sz + 3 }, Color3(0, 0, 0, 145));
			m_pSolidBrush->SetColor(D2D1::ColorF(clr.r / 255.f, clr.g / 255.f, clr.b / 255.f, clr.a / 255.f));

			m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y), dwrite_layout, m_pSolidBrush);
			dwrite_layout->Release( );
			return;
		}


		m_pSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));

		if (outline) {
			const auto x = pos.x;
			const auto y = pos.y;

			m_pCanvas->DrawTextLayout(D2D1::Point2F(x - 1, y), dwrite_layout, m_pSolidBrush);
			m_pCanvas->DrawTextLayout(D2D1::Point2F(x + 1, y), dwrite_layout, m_pSolidBrush);
			m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y - 1), dwrite_layout, m_pSolidBrush);
			m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y + 1), dwrite_layout, m_pSolidBrush);
		}

		m_pSolidBrush->SetColor(D2D1::ColorF(clr.r / 255.f, clr.g / 255.f, clr.b / 255.f, clr.a / 255.f));
		m_pCanvas->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), dwrite_layout, m_pSolidBrush);
		dwrite_layout->Release( );
	}
	template <typename ...Args>
	void boldtext(const Vector2 pos, const Color3 clr, const float sz, bool center, bool outline, const std::wstring_view text, Args&&... args) {
		const auto size = static_cast<std::size_t>(std::swprintf(nullptr, 0, text.data( ), std::forward<Args>(args)...) + 1);

		const std::unique_ptr<wchar_t [ ]> buffer(new wchar_t[ size ]);
		std::swprintf(buffer.get( ), size, text.data( ), std::forward<Args>(args)...);

		const auto str = std::wstring(buffer.get( ), buffer.get( ) + size - 1);
		const auto str_len = static_cast<std::uint32_t>(str.size( ));

		IDWriteTextLayout* dwrite_layout = nullptr;
		RET_CHK(m_pTextEngine->CreateTextLayout(str.c_str( ), str_len, m_pTextFormat, screen_size.x, screen_size.y, &dwrite_layout));

		const DWRITE_TEXT_RANGE range
		{
			0,
			str_len
		};

		dwrite_layout->SetFontSize(sz, range);
		dwrite_layout->SetFontWeight(DWRITE_FONT_WEIGHT_EXTRA_BOLD, range);

		if (center) {
			m_pSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			DWRITE_TEXT_METRICS TextInfo;
			dwrite_layout->GetMetrics(&TextInfo);
			Vector2 TextSize = { TextInfo.width / 2.f, TextInfo.height / 2.f };
			const auto x = pos.x - TextSize.x;
			const auto y = pos.y - TextSize.y;
			if (outline) {

				m_pCanvas->DrawTextLayout(D2D1::Point2F(x - 1, y), dwrite_layout, m_pSolidBrush);
				m_pCanvas->DrawTextLayout(D2D1::Point2F(x + 1, y), dwrite_layout, m_pSolidBrush);
				m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y - 1), dwrite_layout, m_pSolidBrush);
				m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y + 1), dwrite_layout, m_pSolidBrush);
			}

			m_pSolidBrush->SetColor(D2D1::ColorF(clr.r / 255.f, clr.g / 255.f, clr.b / 255.f, clr.a / 255.f));

			m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y), dwrite_layout, m_pSolidBrush);
			dwrite_layout->Release( );
			return;
		}


		m_pSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));

		if (outline) {
			const auto x = pos.x;
			const auto y = pos.y;

			m_pCanvas->DrawTextLayout(D2D1::Point2F(x - 1, y), dwrite_layout, m_pSolidBrush);
			m_pCanvas->DrawTextLayout(D2D1::Point2F(x + 1, y), dwrite_layout, m_pSolidBrush);
			m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y - 1), dwrite_layout, m_pSolidBrush);
			m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y + 1), dwrite_layout, m_pSolidBrush);
		}

		m_pSolidBrush->SetColor(D2D1::ColorF(clr.r / 255.f, clr.g / 255.f, clr.b / 255.f, clr.a / 255.f));
		m_pCanvas->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), dwrite_layout, m_pSolidBrush);
		dwrite_layout->Release( );
	}
	template <typename ...Args>
	void t(const Vector2 pos, const D2D1::ColorF clr, bool center, bool outline, const std::wstring_view text, Args&&... args) {
		const auto size = static_cast<std::size_t>(std::swprintf(nullptr, 0, text.data(), std::forward<Args>(args)...) + 1);

		const std::unique_ptr<wchar_t[]> buffer(new wchar_t[size]);
		std::swprintf(buffer.get(), size, text.data(), std::forward<Args>(args)...);

		const auto str = std::wstring(buffer.get(), buffer.get() + size - 1);
		const auto str_len = static_cast<std::uint32_t>(str.size());

		IDWriteTextLayout* dwrite_layout = nullptr;
		RET_CHK(m_pTextEngine->CreateTextLayout(str.c_str(), str_len, m_pTextFormat, screen_size.x, screen_size.y, &dwrite_layout));

		const DWRITE_TEXT_RANGE range
		{
			0,
			str_len
		};

		dwrite_layout->SetFontSize(12.f, range);

		if (center) {
			m_pSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			DWRITE_TEXT_METRICS TextInfo;
			dwrite_layout->GetMetrics(&TextInfo);
			Vector2 TextSize = { TextInfo.width / 2.f, TextInfo.height / 2.f };
			const auto x = pos.x - TextSize.x;
			const auto y = pos.y - TextSize.y;
			if (outline) {

				m_pCanvas->DrawTextLayout(D2D1::Point2F(x - 1, y), dwrite_layout, m_pSolidBrush);
				m_pCanvas->DrawTextLayout(D2D1::Point2F(x + 1, y), dwrite_layout, m_pSolidBrush);
				m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y - 1), dwrite_layout, m_pSolidBrush);
				m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y + 1), dwrite_layout, m_pSolidBrush);
			}

			m_pSolidBrush->SetColor(clr);

			m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y), dwrite_layout, m_pSolidBrush);
			dwrite_layout->Release();
			return;
		}


		m_pSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));

		if (outline) {
			const auto x = pos.x;
			const auto y = pos.y;

			m_pCanvas->DrawTextLayout(D2D1::Point2F(x - 1, y), dwrite_layout, m_pSolidBrush);
			m_pCanvas->DrawTextLayout(D2D1::Point2F(x + 1, y), dwrite_layout, m_pSolidBrush);
			m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y - 1), dwrite_layout, m_pSolidBrush);
			m_pCanvas->DrawTextLayout(D2D1::Point2F(x, y + 1), dwrite_layout, m_pSolidBrush);
		}

		m_pSolidBrush->SetColor(clr);
		m_pCanvas->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), dwrite_layout, m_pSolidBrush);
		dwrite_layout->Release();
	}
	void end_frame( ) {
		HRESULT state = m_pCanvas->EndDraw( );
		if (state == D2DERR_RECREATE_TARGET)
			reset_canvas( );
	}
}