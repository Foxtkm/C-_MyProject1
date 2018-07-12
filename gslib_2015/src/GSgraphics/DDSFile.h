#ifndef _DDS_FILE_H_
#define _DDS_FILE_H_

#include <vector>
#include <fstream>

// DDSファイルクラス
class DDSFile {
public:
    // 符号無し8bit
    typedef unsigned char BYTE;
    // 符号無し32bit
    typedef unsigned int DWORD;
    // ピクセルフォーマット
    struct PixelFormat {
        DWORD           dwSize;
        DWORD           dwFlags;
        DWORD           dwFourCC;
        DWORD           dwRGBBitCount;
        DWORD           dwRBitMask;
        DWORD           dwGBitMask;
        DWORD           dwBBitMask;
        DWORD           dwRGBAlphaBitMask;
    };
    // 補助フラグ
    struct Caps2 {
        DWORD           dwCaps1;
        DWORD           dwCaps2;
        DWORD           dwReserved[2];
    };
    // サーフェースディスクプリタ
    struct SurfaceDesc {
        DWORD           dwSize;
        DWORD           dwFlags;
        DWORD           dwHeight;
        DWORD           dwWidth;
        DWORD           dwPitchOrLinearSize;
        DWORD           dwDepth;
        DWORD           dwMipMapCount;
        DWORD           dwReserved1[11];
        PixelFormat     ddpfPixelFormat;
        Caps2           ddsCaps;
        DWORD           dwReserved2;
    };
	// DX10拡張ヘッダ
	struct Dx10 {
		DWORD			dwFormat;
		DWORD			dwDimension;
		DWORD			dwMiscFlag;
		DWORD			dwArraySize;
		DWORD			dwMiscFlag2;
	};
	// ＤＤＳフラグ
    static const DWORD FLAG_CAPS          = 0x00000001;
    static const DWORD FLAG_HEIGHT        = 0x00000002;
    static const DWORD FLAG_WIDTH         = 0x00000004;
    static const DWORD FLAG_PITCH         = 0x00000008;
    static const DWORD FLAG_PIXELFORMAT   = 0x00001000;
    static const DWORD FLAG_MIPMAPCOUNT   = 0x00020000;
    static const DWORD FLAG_LINEARSIZE    = 0x00080000;
    static const DWORD FLAG_DEPTH         = 0x00800000;
    // ピクセルフォーマット
    static const DWORD FORMAT_LUMINANCE     = 0x00020000;
    static const DWORD FORMAT_RGB           = 0x00000040;
	static const DWORD FORMAT_ALPHA         = 0x00000002;
	static const DWORD FORMAT_ALPHAPIXELS   = 0x00000001;
	static const DWORD FORMAT_RGBA          = 0x00000041;
	static const DWORD FORMAT_YUV           = 0x00000200;
	static const DWORD FORMAT_FOURCC        = 0x00000004;
    static const DWORD FORMAT_DXT1          = 0x31545844;
    static const DWORD FORMAT_DXT3          = 0x33545844;
    static const DWORD FORMAT_DXT5          = 0x35545844;
	static const DWORD FORMAT_DX10          = 0x30315844;
	static const DWORD FORMAT_A16B16G16R16  = 0x00000024;
	static const DWORD FORMAT_Q16W16V16U16  = 0x0000006e;
	static const DWORD FORMAT_R16F          = 0x0000006f;
	static const DWORD FORMAT_G16R16F       = 0x00000070;
	static const DWORD FORMAT_A16B16G16R16F = 0x00000071;
	static const DWORD FORMAT_R32F          = 0x00000072;
	static const DWORD FORMAT_G32R32F       = 0x00000073;
	static const DWORD FORMAT_A32B32G32R32F = 0x00000074;
	static const DWORD FORMAT_CxV8U8        = 0x00000075;
	static const DWORD FORMAT_Q8W8V8U8      = 0x0000003f;


	// 補助フラグ
	static const DWORD CAPS1_COMPLEX      = 0x00000008;
	static const DWORD CAPS1_MIPMAP       = 0x00400000;
    static const DWORD CAPS1_TEXTURE      = 0x00001000;
    static const DWORD CAPS2_CUBEMAP      = 0x0000fe00;
    static const DWORD CAPS2_VOLUME       = 0x00200000;
	// D3D10_RESORCR_DIMENSION
	static const DWORD	DDS10_DIMENSION_1D = 2;
	static const DWORD	DDS10_DIMENSION_2D = 3;
	static const DWORD	DDS10_DIMENSION_3D = 4;
	// DX10ピクセルフォーマット
	enum {
		DXGI_FORMAT_UNKNOWN = 0,
		DXGI_FORMAT_R32G32B32A32_TYPELESS = 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT = 2,
		DXGI_FORMAT_R32G32B32A32_UINT = 3,
		DXGI_FORMAT_R32G32B32A32_SINT = 4,
		DXGI_FORMAT_R32G32B32_TYPELESS = 5,
		DXGI_FORMAT_R32G32B32_FLOAT = 6,
		DXGI_FORMAT_R32G32B32_UINT = 7,
		DXGI_FORMAT_R32G32B32_SINT = 8,
		DXGI_FORMAT_R16G16B16A16_TYPELESS = 9,
		DXGI_FORMAT_R16G16B16A16_FLOAT = 10,
		DXGI_FORMAT_R16G16B16A16_UNORM = 11,
		DXGI_FORMAT_R16G16B16A16_UINT = 12,
		DXGI_FORMAT_R16G16B16A16_SNORM = 13,
		DXGI_FORMAT_R16G16B16A16_SINT = 14,
		DXGI_FORMAT_R32G32_TYPELESS = 15,
		DXGI_FORMAT_R32G32_FLOAT = 16,
		DXGI_FORMAT_R32G32_UINT = 17,
		DXGI_FORMAT_R32G32_SINT = 18,
		DXGI_FORMAT_R32G8X24_TYPELESS = 19,
		DXGI_FORMAT_D32_FLOAT_S8X24_UINT = 20,
		DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
		DXGI_FORMAT_X32_TYPELESS_G8X24_UINT = 22,
		DXGI_FORMAT_R10G10B10A2_TYPELESS = 23,
		DXGI_FORMAT_R10G10B10A2_UNORM = 24,
		DXGI_FORMAT_R10G10B10A2_UINT = 25,
		DXGI_FORMAT_R11G11B10_FLOAT = 26,
		DXGI_FORMAT_R8G8B8A8_TYPELESS = 27,
		DXGI_FORMAT_R8G8B8A8_UNORM = 28,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
		DXGI_FORMAT_R8G8B8A8_UINT = 30,
		DXGI_FORMAT_R8G8B8A8_SNORM = 31,
		DXGI_FORMAT_R8G8B8A8_SINT = 32,
		DXGI_FORMAT_R16G16_TYPELESS = 33,
		DXGI_FORMAT_R16G16_FLOAT = 34,
		DXGI_FORMAT_R16G16_UNORM = 35,
		DXGI_FORMAT_R16G16_UINT = 36,
		DXGI_FORMAT_R16G16_SNORM = 37,
		DXGI_FORMAT_R16G16_SINT = 38,
		DXGI_FORMAT_R32_TYPELESS = 39,
		DXGI_FORMAT_D32_FLOAT = 40,
		DXGI_FORMAT_R32_FLOAT = 41,
		DXGI_FORMAT_R32_UINT = 42,
		DXGI_FORMAT_R32_SINT = 43,
		DXGI_FORMAT_R24G8_TYPELESS = 44,
		DXGI_FORMAT_D24_UNORM_S8_UINT = 45,
		DXGI_FORMAT_R24_UNORM_X8_TYPELESS = 46,
		DXGI_FORMAT_X24_TYPELESS_G8_UINT = 47,
		DXGI_FORMAT_R8G8_TYPELESS = 48,
		DXGI_FORMAT_R8G8_UNORM = 49,
		DXGI_FORMAT_R8G8_UINT = 50,
		DXGI_FORMAT_R8G8_SNORM = 51,
		DXGI_FORMAT_R8G8_SINT = 52,
		DXGI_FORMAT_R16_TYPELESS = 53,
		DXGI_FORMAT_R16_FLOAT = 54,
		DXGI_FORMAT_D16_UNORM = 55,
		DXGI_FORMAT_R16_UNORM = 56,
		DXGI_FORMAT_R16_UINT = 57,
		DXGI_FORMAT_R16_SNORM = 58,
		DXGI_FORMAT_R16_SINT = 59,
		DXGI_FORMAT_R8_TYPELESS = 60,
		DXGI_FORMAT_R8_UNORM = 61,
		DXGI_FORMAT_R8_UINT = 62,
		DXGI_FORMAT_R8_SNORM = 63,
		DXGI_FORMAT_R8_SINT = 64,
		DXGI_FORMAT_A8_UNORM = 65,
		DXGI_FORMAT_R1_UNORM = 66,
		DXGI_FORMAT_R9G9B9E5_SHAREDEXP = 67,
		DXGI_FORMAT_R8G8_B8G8_UNORM = 68,
		DXGI_FORMAT_G8R8_G8B8_UNORM = 69,
		DXGI_FORMAT_BC1_TYPELESS = 70,
		DXGI_FORMAT_BC1_UNORM = 71,
		DXGI_FORMAT_BC1_UNORM_SRGB = 72,
		DXGI_FORMAT_BC2_TYPELESS = 73,
		DXGI_FORMAT_BC2_UNORM = 74,
		DXGI_FORMAT_BC2_UNORM_SRGB = 75,
		DXGI_FORMAT_BC3_TYPELESS = 76,
		DXGI_FORMAT_BC3_UNORM = 77,
		DXGI_FORMAT_BC3_UNORM_SRGB = 78,
		DXGI_FORMAT_BC4_TYPELESS = 79,
		DXGI_FORMAT_BC4_UNORM = 80,
		DXGI_FORMAT_BC4_SNORM = 81,
		DXGI_FORMAT_BC5_TYPELESS = 82,
		DXGI_FORMAT_BC5_UNORM = 83,
		DXGI_FORMAT_BC5_SNORM = 84,
		DXGI_FORMAT_B5G6R5_UNORM = 85,
		DXGI_FORMAT_B5G5R5A1_UNORM = 86,
		DXGI_FORMAT_B8G8R8A8_UNORM = 87,
		DXGI_FORMAT_B8G8R8X8_UNORM = 88,
		DXGI_FORMAT_FORCE_UINT = 0xffffffffUL
	};

public:
    // コンストラクタ
	explicit DDSFile(const char* fileName) {
		load(fileName);
	}
    // コンストラクタ
	explicit DDSFile(const DDSFile::SurfaceDesc& desc) :
		desc_(desc) {
	}
	// コンストラクタ
	explicit DDSFile(const DDSFile::SurfaceDesc& desc, const DDSFile::Dx10& dx10) :
		desc_(desc), dx10_header_(dx10) {
	}
	// ファイルから読み込む
	void load(const char* fileName) {
		std::ifstream f;
		// 例外を有効にする
		f.exceptions(std::ifstream::eofbit | std::ifstream::failbit | std::ifstream::badbit);
		// ファイルオープン
		f.open(fileName, std::ifstream::in | std::ifstream::binary);
		// ファイルサイズを求める
		f.seekg(0, std::ios::end);
		DWORD fileSize = (DWORD)f.tellg();
		f.seekg(0, std::ios::beg);
		// マジックナンバーの読み込み
		DWORD magicNumber = 0;
		f.read(reinterpret_cast<char*>(&magicNumber), sizeof(DWORD));
		if (magicNumber != DDS_MAGIC_NUMBER) {
			throw std::ifstream::failure("DDS File Error");
		}
		// ディクスプリタの読み込み
		f.read(reinterpret_cast<char*>(&desc_), sizeof(SurfaceDesc));
		// DX10ヘッダの読み込み
		if (desc_.ddpfPixelFormat.dwFourCC == FORMAT_DX10) {
			f.read(reinterpret_cast<char*>(&dx10_header_), sizeof(Dx10));
		}
		// サーフェースサイズの計算
		unsigned int surfaceSize = fileSize - sizeof(DWORD) - sizeof(SurfaceDesc);
		surfaceSize -= (desc_.ddpfPixelFormat.dwFourCC == FORMAT_DX10) ? sizeof(Dx10) : 0;
		// サーフェースデータの読み込み
		resize(surfaceSize);
		f.read(reinterpret_cast<char*>(&surface_[0]), (std::streamsize)size());
	}
    // ファイルに書き込む
	void save(const char* fileName) {
		std::ofstream f;
		// 例外を有効にする
		f.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		// ファイルオープン
		f.open(fileName, std::ofstream::out | std::ofstream::binary);
		// マジックナンバーの書き込み
		DWORD magicNumber = DDS_MAGIC_NUMBER;
		f.write(reinterpret_cast<char*>(&magicNumber), sizeof(DWORD));
		// ディクスプリタの書き込み
		f.write(reinterpret_cast<char*>(&desc_), sizeof(SurfaceDesc));
		// DX10ヘッダの書き込み
		if (desc_.ddpfPixelFormat.dwFourCC == FORMAT_DX10) {
			f.write(reinterpret_cast<char*>(&dx10_header_), sizeof(Dx10));
		}
		// サーフェースデータの書き込み
		f.write(reinterpret_cast<char*>(&surface_[0]), (std::streamsize)surface_.size());
	}
    // ディスクプリタの取得 
	const SurfaceDesc& desc() const {
		return desc_;
	}
	// DX10ヘッダの取得 
	const Dx10& dx10_header() const {
		return dx10_header_;
	}
	// サイズの取得
	unsigned int size() const {
		return (unsigned int)surface_.size();
	}
    // リサイズ
	void resize(unsigned int size) {
		surface_.resize(size);
	}
	// サーフェースデータの取得
	DDSFile::BYTE& operator[] (unsigned int index) {
		return surface_[index];
	}
	// サーフェースデータの取得
	const DDSFile::BYTE& operator[] (unsigned int index) const {
		return surface_[index];
	}
private:
    // ＤＤＳファイルマジックナンバ
    static const DWORD DDS_MAGIC_NUMBER = 0x20534444;
    // サーフェース記述子
    SurfaceDesc desc_;
	// DirectX10拡張ヘッダ
	Dx10		dx10_header_;
    // サーフェースデータ
    typedef std::vector<BYTE> Surface;
    Surface     surface_;
};

#endif

// end of file
