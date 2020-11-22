#ifndef _MSC_VER

#pragma GCC optimize "O3,omit-frame-pointer,inline"

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else

#define likely(x) x
#define unlikely(x) x

#endif

#define REDIRECT_

#include <iostream>
#include <string>
#include <sstream>

#include <vector>
#include <array>

#include <set>
#include <map>
#include <queue>
#include <deque>

#include <algorithm>
#include <cmath>

#include <chrono>
#include <memory>

#pragma region ライブラリ

#define forange(counter, end) forstep(counter, 0, end)
#define forstep(counter, begin, end) forstep_type(size_t, counter, begin, end)
#define forange_type(type, counter, end) forstep_type(type, counter, 0, end)
#define forstep_type(type, counter, begin, end) for (type counter = begin, forstep_type_end_##counter = end; counter < forstep_type_end_##counter; counter++)

void errerLine(const std::string &mes)
{
	std::cerr << "@" << mes << std::endl;
}

/**
 * @brief 時間計測を行うクラス
 *
 */
class Stopwatch
{
public:
	/**
	 * @brief Construct a new Stopwatch object
	 *
	 */
	Stopwatch() = default;

	/**
	 * @brief 計測を開始する
	 *
	 */
	inline void start() noexcept
	{
		s = std::chrono::high_resolution_clock::now();
		e = s;
	}
	/**
	 * @brief 計測を停止する
	 *
	 */
	inline void stop() noexcept
	{
		e = std::chrono::high_resolution_clock::now();
	}

	/**
	 * @brief 計測時間を取得する(ナノ秒)
	 *
	 * @return const long long 計測時間(ナノ秒)
	 */
	inline const long long nanoseconds() const noexcept { return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count(); }
	/**
	 * @brief 計測時間を取得する(マイクロ秒)
	 *
	 * @return const long long 計測時間(マイクロ秒)
	 */
	inline const long long microseconds() const noexcept { return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count(); }
	/**
	 * @brief 計測時間を取得する(ミリ秒)
	 *
	 * @return const long long 計測時間(ミリ秒)
	 */
	inline const long long millisecond() const noexcept { return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count(); }

	/**
	 * @brief 単位付きの計測時間の文字列を得る(ナノ秒)
	 *
	 * @return const std::string 計測時間の文字列(ナノ秒)
	 */
	inline const std::string toString_ns() const { return std::to_string(nanoseconds()) + "ns"; }
	/**
	 * @brief 単位付きの計測時間の文字列を得る(マイクロ秒)
	 *
	 * @return const std::string 計測時間の文字列(マイクロ秒)
	 */
	inline const std::string toString_us() const { return std::to_string(microseconds()) + "us"; }
	/**
	 * @brief 単位付きの計測時間の文字列を得る(ミリ秒)
	 *
	 * @return const std::string 計測時間の文字列(ミリ秒)
	 */
	inline const std::string toString_ms() const { return std::to_string(millisecond()) + "ms"; }

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	std::chrono::time_point<std::chrono::high_resolution_clock> e;
};

/**
 * @brief 一定時間の経過を確認するクラス
 *
 */
class MilliSecTimer
{
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	long long time = 0;

public:
	/**
	 * @brief Construct a new Milli Sec Timer object
	 *
	 */
	MilliSecTimer() = default;
	/**
	 * @brief Construct a new Milli Sec Timer object
	 *
	 * @param _time 設定時間(ミリ秒)
	 */
	MilliSecTimer(const std::chrono::milliseconds &_time) noexcept { time = _time.count(); }

	/**
	 * @brief 時間を設定する
	 *
	 * @param _time 設定時間(ミリ秒)
	 */
	void set(const std::chrono::milliseconds &_time) noexcept { time = _time.count(); }

	/**
	 * @brief タイマーを開始させる
	 *
	 */
	void start() noexcept
	{
		s = std::chrono::high_resolution_clock::now();
	}

	/**
	 * @brief 設定時間経過したかを得る
	 *
	 * @return true 設定時間を経過している
	 * @return false 設定時間以内
	 */
	inline const bool check() const noexcept
	{
		const auto e = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count() >= time;
	}

	/**
	 * @brief 設定時間経過したかを得る
	 *
	 * @return true 設定時間を経過している
	 * @return false 設定時間以内
	 */
	operator bool() const noexcept { return check(); }

	/**
	 * @brief 経過時間を取得する(ミリ秒)
	 *
	 * @return const long long 経過時間(ミリ秒)
	 */
	inline const long long interval() const noexcept
	{
		const auto e = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count();
	}
};

template <class Type, size_t Size>
class MemoryPool
{
private:
	using Deque = std::deque<Type *>;

	Type *m_data = nullptr;

	bool isRecycle = false;
	size_t pointer = 0;
	Deque addr;

	MemoryPool()
	{
		uint64_t memorySize = sizeof(Type) * Size;
		m_data = static_cast<Type *>(std::malloc(memorySize));

		errerLine("Memory Size:" + std::to_string(memorySize / 1024.0 / 1024.0) + "MB");
	}

public:
	inline static std::shared_ptr<MemoryPool<Type, Size>> instance{};

	static void Create()
	{
		instance.reset(new MemoryPool());
	}

	~MemoryPool()
	{
		if (m_data != nullptr)
			std::free(m_data);
	}

	void clear()
	{
		isRecycle = false;
		pointer = 0;
		addr.clear();
	}

	void *get()
	{
		if (unlikely(isRecycle))
		{
			if (unlikely(addr.empty()))
			{
				errerLine("MemoryPool out of range");
				return nullptr;
			}

			const auto p = addr.back();
			addr.pop_back();

			return p;
		}
		else
		{
			pointer++;
			if (pointer >= Size)
			{
				isRecycle = true;
			}

			return &m_data[pointer - 1];
		}
	}

	void release(Type *p)
	{
		addr.push_back(p);
	}
};

template <size_t Size>
class EvaluateExp
{
private:
	std::array<double, Size> m_data{};

public:
	EvaluateExp()
	{
		forange(x, Size)
		{
			m_data[x] = exp(-static_cast<double>(x) / static_cast<double>(Size * 1));
		}
	}

	[[nodiscard]] double operator[](const size_t index) const
	{
		return m_data[index];
	}

	constexpr size_t size() const
	{
		return m_data.size();
	}
};

/**
 * @brief xoshiro128++
 * 
 */
class XoShiro128
{
public:
	using value_type = uint32_t;

private:
	value_type m_state0 = 723471715u;
	value_type m_state1 = 2497366906u;
	value_type m_state2 = 2064144800u;
	value_type m_state3 = 2008045182u;

public:
	XoShiro128() {}

	/**
	 * @brief 32bitの乱数値を取得する
	 * 
	 * @return value_type 
	 */
	[[nodiscard]] value_type next()
	{
		const value_type rotl_tmp = m_state0 + m_state3;
		const value_type result = ((rotl_tmp << 7) | (rotl_tmp >> (32 - 7))) + m_state0;

		const value_type t = m_state1 << 9;

		m_state2 ^= m_state0;
		m_state3 ^= m_state1;
		m_state1 ^= m_state2;
		m_state0 ^= m_state3;

		m_state2 ^= t;

		m_state3 = ((m_state3 << 11) | (m_state3 >> (32 - 11)));

		return result;
	}

	/**
	 * @brief [0.0, 1.0)の範囲の乱数値を取得する
	 * 
	 * @return float [0.0, 1.0)
	 */
	[[nodiscard]] float nextFloat()
	{
		const value_type v = next();

		return static_cast<float>((v >> 8) * 0x1.0p-24f);
	}
	/**
	 * @brief [0.0, 1.0)の範囲の乱数値を取得する
	 * 
	 * @return double [0.0, 1.0)
	 */
	[[nodiscard]] double nextDouble()
	{
		const value_type v1 = next();
		const value_type v2 = next();
		const uint64_t v = (static_cast<uint64_t>(v1) << 32) | static_cast<uint64_t>(v2);

		return static_cast<double>((v >> 11) * 0x1.0p-53);
	}
};

#pragma endregion

#pragma region 定数宣言

namespace Object
{
	const std::string RoundActionCast = "CAST";
	const std::string RoundActionOpponentCast = "OPPONENT_CAST";
	const std::string RoundActionLearn = "LEARN";
	const std::string RoundActionBrew = "BREW";
	const std::string RoundActionRest = "REST";
	const std::string RoundActionWait = "WAIT";

	const int InventorySize = 10;
	const int PotionLimit = 6;

	enum class Operation : char
	{
		Brew,
		Cast,
		Learn,
		Rest,
		Wait
	};

} // namespace Object

#pragma endregion

#pragma region データ構造

struct Tier
{
	using value_type = char;

	value_type tier0 = value_type();
	value_type tier1 = value_type();
	value_type tier2 = value_type();
	value_type tier3 = value_type();

	constexpr Tier() {}
	constexpr Tier(value_type tier0, value_type tier1, value_type tier2, value_type tier3)
		: tier0(tier0), tier1(tier1), tier2(tier2), tier3(tier3)
	{
	}

	constexpr Tier getPlus() const
	{
		const value_type t0 = tier0 > 0 ? tier0 : 0;
		const value_type t1 = tier1 > 0 ? tier1 : 0;
		const value_type t2 = tier2 > 0 ? tier2 : 0;
		const value_type t3 = tier3 > 0 ? tier3 : 0;

		return Tier(t0, t1, t2, t3);
	}

	value_type getSum() const
	{
		return tier0 + tier1 + tier2 + tier3;
	}

	value_type getScore() const
	{
		return tier1 + tier2 + tier3;
	}

	constexpr Tier operator+(const Tier &o) const
	{
		return Tier(
			tier0 + o.tier0,
			tier1 + o.tier1,
			tier2 + o.tier2,
			tier3 + o.tier3);
	}
	void operator+=(const Tier &o)
	{
		tier0 += o.tier0;
		tier1 += o.tier1;
		tier2 += o.tier2;
		tier3 += o.tier3;
	}

	constexpr bool operator==(const Tier &o) const
	{
		return (tier0 == o.tier0 && tier1 == o.tier1 && tier2 == o.tier2 && tier3 == o.tier3);
	}

	constexpr bool operator<(const Tier &o) const
	{
		if (tier0 != o.tier0)
			return tier0 < o.tier0;
		if (tier1 != o.tier1)
			return tier1 < o.tier1;
		if (tier2 != o.tier2)
			return tier2 < o.tier2;
		return tier3 < o.tier3;
	}

	bool isAccept(const Tier &o) const
	{
		auto p = (*this) + o;

		if (0 > p.tier0)
			return false;
		if (0 > p.tier1)
			return false;
		if (0 > p.tier2)
			return false;
		if (0 > p.tier3)
			return false;

		if (p.getSum() > Object::InventorySize)
			return false;

		return true;
	}
};

struct Magic
{
	/**
	 * @brief ユニークID
	 *
	 */
	int actionId = 0; // the unique ID of this spell or recipe
	/**
	 * @brief 素材コスト
	 *
	 */
	Tier delta; // tier ingredient change
	/**
	 * @brief ポーションの値段
	 *
	 */
	int price = 0; // the price in rupees if this is a potion
	/**
	 * @brief スペルの先読み税
	 *
	 */
	int tomeIndex = 0; // the index in the tome if this is a tome spell, equal to the read-ahead tax
	/**
	 * @brief スペル取得時に入手するTier0素材数
	 *
	 */
	int taxCount = 0; // the amount of taxed tier-0 ingredients you gain from learning this spell
	/**
	 * @brief スペルの使用可否
	 * true:使用可能、false:使用不可
	 */
	bool castable = false; // 1 if this is a castable player spell
	/**
	 * @brief スペルの連続使用可否
	 * true:使用可能、false:使用不可
	 */
	bool repeatable = false; // 1 if this is a repeatable player spell
};

struct Inventory
{
	Tier inv;	   // tier ingredient change
	int score = 0; // amount of rupees
};

constexpr Magic ID00{0, Tier(-3, 0, 0, 1), 0, -1, -1, false, true};
constexpr Magic ID01{1, Tier(3, -1, 0, 0), 0, -1, -1, false, true};
constexpr Magic ID02{2, Tier(1, 1, 0, 0), 0, -1, -1, false, false};
constexpr Magic ID03{3, Tier(0, 0, 1, 0), 0, -1, -1, false, false};
constexpr Magic ID04{4, Tier(3, 0, 0, 0), 0, -1, -1, false, false};
constexpr Magic ID05{5, Tier(2, 3, -2, 0), 0, -1, -1, false, true};
constexpr Magic ID06{6, Tier(2, 1, -2, 1), 0, -1, -1, false, true};
constexpr Magic ID07{7, Tier(3, 0, 1, -1), 0, -1, -1, false, true};
constexpr Magic ID08{8, Tier(3, -2, 1, 0), 0, -1, -1, false, true};
constexpr Magic ID09{9, Tier(2, -3, 2, 0), 0, -1, -1, false, true};
constexpr Magic ID10{10, Tier(2, 2, 0, -1), 0, -1, -1, false, true};
constexpr Magic ID11{11, Tier(-4, 0, 2, 0), 0, -1, -1, false, true};
constexpr Magic ID12{12, Tier(2, 1, 0, 0), 0, -1, -1, false, false};
constexpr Magic ID13{13, Tier(4, 0, 0, 0), 0, -1, -1, false, false};
constexpr Magic ID14{14, Tier(0, 0, 0, 1), 0, -1, -1, false, false};
constexpr Magic ID15{15, Tier(0, 2, 0, 0), 0, -1, -1, false, false};
constexpr Magic ID16{16, Tier(1, 0, 1, 0), 0, -1, -1, false, false};
constexpr Magic ID17{17, Tier(-2, 0, 1, 0), 0, -1, -1, false, true};
constexpr Magic ID18{18, Tier(-1, -1, 0, 1), 0, -1, -1, false, true};
constexpr Magic ID19{19, Tier(0, 2, -1, 0), 0, -1, -1, false, true};
constexpr Magic ID20{20, Tier(2, -2, 0, 1), 0, -1, -1, false, true};
constexpr Magic ID21{21, Tier(-3, 1, 1, 0), 0, -1, -1, false, true};
constexpr Magic ID22{22, Tier(0, 2, -2, 1), 0, -1, -1, false, true};
constexpr Magic ID23{23, Tier(1, -3, 1, 1), 0, -1, -1, false, true};
constexpr Magic ID24{24, Tier(0, 3, 0, -1), 0, -1, -1, false, true};
constexpr Magic ID25{25, Tier(0, -3, 0, 2), 0, -1, -1, false, true};
constexpr Magic ID26{26, Tier(1, 1, 1, -1), 0, -1, -1, false, true};
constexpr Magic ID27{27, Tier(1, 2, -1, 0), 0, -1, -1, false, true};
constexpr Magic ID28{28, Tier(4, 1, -1, 0), 0, -1, -1, false, true};
constexpr Magic ID29{29, Tier(-5, 0, 0, 2), 0, -1, -1, false, true};
constexpr Magic ID30{30, Tier(-4, 0, 1, 1), 0, -1, -1, false, true};
constexpr Magic ID31{31, Tier(0, 3, 2, -2), 0, -1, -1, false, true};
constexpr Magic ID32{32, Tier(1, 1, 3, -2), 0, -1, -1, false, true};
constexpr Magic ID33{33, Tier(-5, 0, 3, 0), 0, -1, -1, false, true};
constexpr Magic ID34{34, Tier(-2, 0, -1, 2), 0, -1, -1, false, true};
constexpr Magic ID35{35, Tier(0, 0, -3, 3), 0, -1, -1, false, true};
constexpr Magic ID36{36, Tier(0, -3, 3, 0), 0, -1, -1, false, true};
constexpr Magic ID37{37, Tier(-3, 3, 0, 0), 0, -1, -1, false, true};
constexpr Magic ID38{38, Tier(-2, 2, 0, 0), 0, -1, -1, false, true};
constexpr Magic ID39{39, Tier(0, 0, -2, 2), 0, -1, -1, false, true};
constexpr Magic ID40{40, Tier(0, -2, 2, 0), 0, -1, -1, false, true};
constexpr Magic ID41{41, Tier(0, 0, 2, -1), 0, -1, -1, false, true};

constexpr Magic ID78{42, Tier(2, 0, 0, 0), 0, -1, -1, true, false};
constexpr Magic ID79{43, Tier(-1, 1, 0, 0), 0, -1, -1, true, false};
constexpr Magic ID80{44, Tier(0, -1, 1, 0), 0, -1, -1, true, false};
constexpr Magic ID81{45, Tier(0, 0, -1, 1), 0, -1, -1, true, false};

constexpr Magic ID42{42, Tier(-2, -2, -0, -0), 6, -1, -1, false, false};
constexpr Magic ID43{43, Tier(-3, -2, -0, -0), 7, -1, -1, false, false};
constexpr Magic ID44{44, Tier(-0, -4, -0, -0), 8, -1, -1, false, false};
constexpr Magic ID45{45, Tier(-2, -0, -2, -0), 8, -1, -1, false, true};
constexpr Magic ID46{46, Tier(-2, -3, -0, -0), 8, -1, -1, false, true};
constexpr Magic ID47{47, Tier(-3, -0, -2, -0), 9, -1, -1, false, true};
constexpr Magic ID48{48, Tier(-0, -2, -2, -0), 10, -1, -1, false, true};
constexpr Magic ID49{49, Tier(-0, -5, -0, -0), 10, -1, -1, false, true};
constexpr Magic ID50{50, Tier(-2, -0, -0, -2), 10, -1, -1, false, true};
constexpr Magic ID51{51, Tier(-2, -0, -3, -0), 11, -1, -1, false, true};
constexpr Magic ID52{52, Tier(-3, -0, -0, -2), 11, -1, -1, false, false};
constexpr Magic ID53{53, Tier(-0, -0, -4, -0), 12, -1, -1, false, false};
constexpr Magic ID54{54, Tier(-0, -2, -0, -2), 12, -1, -1, false, false};
constexpr Magic ID55{55, Tier(-0, -3, -2, -0), 12, -1, -1, false, false};
constexpr Magic ID56{56, Tier(-0, -2, -3, -0), 13, -1, -1, false, false};
constexpr Magic ID57{57, Tier(-0, -0, -2, -2), 14, -1, -1, false, true};
constexpr Magic ID58{58, Tier(-0, -3, -0, -2), 14, -1, -1, false, true};
constexpr Magic ID59{59, Tier(-2, -0, -0, -3), 14, -1, -1, false, true};
constexpr Magic ID60{60, Tier(-0, -0, -5, -0), 15, -1, -1, false, true};
constexpr Magic ID61{61, Tier(-0, -0, -0, -4), 16, -1, -1, false, true};
constexpr Magic ID62{62, Tier(-0, -2, -0, -3), 16, -1, -1, false, true};
constexpr Magic ID63{63, Tier(-0, -0, -3, -2), 17, -1, -1, false, true};
constexpr Magic ID64{64, Tier(-0, -0, -2, -3), 18, -1, -1, false, true};
constexpr Magic ID65{65, Tier(-0, -0, -0, -5), 20, -1, -1, false, true};
constexpr Magic ID66{66, Tier(-2, -1, -0, -1), 9, -1, -1, false, true};
constexpr Magic ID67{67, Tier(-0, -2, -1, -1), 12, -1, -1, false, true};
constexpr Magic ID68{68, Tier(-1, -0, -2, -1), 12, -1, -1, false, true};
constexpr Magic ID69{69, Tier(-2, -2, -2, -0), 13, -1, -1, false, true};
constexpr Magic ID70{70, Tier(-2, -2, -0, -2), 15, -1, -1, false, true};
constexpr Magic ID71{71, Tier(-2, -0, -2, -2), 17, -1, -1, false, true};
constexpr Magic ID72{72, Tier(-0, -2, -2, -2), 19, -1, -1, false, true};
constexpr Magic ID73{73, Tier(-1, -1, -1, -1), 12, -1, -1, false, true};
constexpr Magic ID74{74, Tier(-3, -1, -1, -1), 14, -1, -1, false, true};
constexpr Magic ID75{75, Tier(-1, -3, -1, -1), 16, -1, -1, false, true};
constexpr Magic ID76{76, Tier(-1, -1, -3, -1), 18, -1, -1, false, true};
constexpr Magic ID77{77, Tier(-1, -1, -1, -3), 20, -1, -1, false, true};

constexpr std::array<Magic, 42> LearnSpell = {
	ID00, ID01, ID02, ID03, ID04, ID05, ID06, ID07, ID08, ID09,
	ID10, ID11, ID12, ID13, ID14, ID15, ID16, ID17, ID18, ID19,
	ID20, ID21, ID22, ID23, ID24, ID25, ID26, ID27, ID28, ID29,
	ID30, ID31, ID32, ID33, ID34, ID35, ID36, ID37, ID38, ID39,
	ID40, ID41};
constexpr std::array<Tier, 42 + 4> LearnSpellPlus = {
	ID00.delta.getPlus(),
	ID01.delta.getPlus(),
	ID02.delta.getPlus(),
	ID03.delta.getPlus(),
	ID04.delta.getPlus(),
	ID05.delta.getPlus(),
	ID06.delta.getPlus(),
	ID07.delta.getPlus(),
	ID08.delta.getPlus(),
	ID09.delta.getPlus(),
	ID10.delta.getPlus(),
	ID11.delta.getPlus(),
	ID12.delta.getPlus(),
	ID13.delta.getPlus(),
	ID14.delta.getPlus(),
	ID15.delta.getPlus(),
	ID16.delta.getPlus(),
	ID17.delta.getPlus(),
	ID18.delta.getPlus(),
	ID19.delta.getPlus(),
	ID20.delta.getPlus(),
	ID21.delta.getPlus(),
	ID22.delta.getPlus(),
	ID23.delta.getPlus(),
	ID24.delta.getPlus(),
	ID25.delta.getPlus(),
	ID26.delta.getPlus(),
	ID27.delta.getPlus(),
	ID28.delta.getPlus(),
	ID29.delta.getPlus(),
	ID30.delta.getPlus(),
	ID31.delta.getPlus(),
	ID32.delta.getPlus(),
	ID33.delta.getPlus(),
	ID34.delta.getPlus(),
	ID35.delta.getPlus(),
	ID36.delta.getPlus(),
	ID37.delta.getPlus(),
	ID38.delta.getPlus(),
	ID39.delta.getPlus(),
	ID40.delta.getPlus(),
	ID41.delta.getPlus(),
	ID78.delta.getPlus(),
	ID79.delta.getPlus(),
	ID80.delta.getPlus(),
	ID81.delta.getPlus()};
constexpr std::array<Magic, 42 + 4> CastSpell = {
	ID00, ID01, ID02, ID03, ID04, ID05, ID06, ID07, ID08, ID09,
	ID10, ID11, ID12, ID13, ID14, ID15, ID16, ID17, ID18, ID19,
	ID20, ID21, ID22, ID23, ID24, ID25, ID26, ID27, ID28, ID29,
	ID30, ID31, ID32, ID33, ID34, ID35, ID36, ID37, ID38, ID39,
	ID40, ID41, ID78, ID79, ID80, ID81};
constexpr std::array<Magic, 36> BrewPostion = {
	ID42, ID43, ID44, ID45, ID46, ID47, ID48, ID49,
	ID50, ID51, ID52, ID53, ID54, ID55, ID56, ID57, ID58, ID59,
	ID60, ID61, ID62, ID63, ID64, ID65, ID66, ID67, ID68, ID69,
	ID70, ID71, ID72, ID73, ID74, ID75, ID76, ID77};

const std::map<Tier, size_t> LearnSpellMap{
	std::make_pair(ID00.delta, 0), std::make_pair(ID01.delta, 1), std::make_pair(ID02.delta, 2), std::make_pair(ID03.delta, 3), std::make_pair(ID04.delta, 4), std::make_pair(ID05.delta, 5), std::make_pair(ID06.delta, 6), std::make_pair(ID07.delta, 7), std::make_pair(ID08.delta, 8), std::make_pair(ID09.delta, 9),
	std::make_pair(ID10.delta, 10), std::make_pair(ID11.delta, 11), std::make_pair(ID12.delta, 12), std::make_pair(ID13.delta, 13), std::make_pair(ID14.delta, 14), std::make_pair(ID15.delta, 15), std::make_pair(ID16.delta, 16), std::make_pair(ID17.delta, 17), std::make_pair(ID18.delta, 18), std::make_pair(ID19.delta, 19),
	std::make_pair(ID20.delta, 20), std::make_pair(ID21.delta, 21), std::make_pair(ID22.delta, 22), std::make_pair(ID23.delta, 23), std::make_pair(ID24.delta, 24), std::make_pair(ID25.delta, 25), std::make_pair(ID26.delta, 26), std::make_pair(ID27.delta, 27), std::make_pair(ID28.delta, 28), std::make_pair(ID29.delta, 29),
	std::make_pair(ID30.delta, 30), std::make_pair(ID31.delta, 31), std::make_pair(ID32.delta, 32), std::make_pair(ID33.delta, 33), std::make_pair(ID34.delta, 34), std::make_pair(ID35.delta, 35), std::make_pair(ID36.delta, 36), std::make_pair(ID37.delta, 37), std::make_pair(ID38.delta, 38), std::make_pair(ID39.delta, 39),
	std::make_pair(ID40.delta, 40), std::make_pair(ID41.delta, 41)};

const std::map<Tier, size_t> CastSpellMap{
	std::make_pair(ID00.delta, 0), std::make_pair(ID01.delta, 1), std::make_pair(ID02.delta, 2), std::make_pair(ID03.delta, 3), std::make_pair(ID04.delta, 4), std::make_pair(ID05.delta, 5), std::make_pair(ID06.delta, 6), std::make_pair(ID07.delta, 7), std::make_pair(ID08.delta, 8), std::make_pair(ID09.delta, 9),
	std::make_pair(ID10.delta, 10), std::make_pair(ID11.delta, 11), std::make_pair(ID12.delta, 12), std::make_pair(ID13.delta, 13), std::make_pair(ID14.delta, 14), std::make_pair(ID15.delta, 15), std::make_pair(ID16.delta, 16), std::make_pair(ID17.delta, 17), std::make_pair(ID18.delta, 18), std::make_pair(ID19.delta, 19),
	std::make_pair(ID20.delta, 20), std::make_pair(ID21.delta, 21), std::make_pair(ID22.delta, 22), std::make_pair(ID23.delta, 23), std::make_pair(ID24.delta, 24), std::make_pair(ID25.delta, 25), std::make_pair(ID26.delta, 26), std::make_pair(ID27.delta, 27), std::make_pair(ID28.delta, 28), std::make_pair(ID29.delta, 29),
	std::make_pair(ID30.delta, 30), std::make_pair(ID31.delta, 31), std::make_pair(ID32.delta, 32), std::make_pair(ID33.delta, 33), std::make_pair(ID34.delta, 34), std::make_pair(ID35.delta, 35), std::make_pair(ID36.delta, 36), std::make_pair(ID37.delta, 37), std::make_pair(ID38.delta, 38), std::make_pair(ID39.delta, 39),
	std::make_pair(ID40.delta, 40), std::make_pair(ID41.delta, 41), std::make_pair(ID78.delta, 42), std::make_pair(ID79.delta, 43), std::make_pair(ID80.delta, 44), std::make_pair(ID81.delta, 45)};

const std::map<Tier, size_t> BrewPotionMap{
	std::make_pair(ID42.delta, 0), std::make_pair(ID43.delta, 1), std::make_pair(ID44.delta, 2), std::make_pair(ID45.delta, 3), std::make_pair(ID46.delta, 4), std::make_pair(ID47.delta, 5), std::make_pair(ID48.delta, 6), std::make_pair(ID49.delta, 7), std::make_pair(ID50.delta, 8), std::make_pair(ID51.delta, 9),
	std::make_pair(ID52.delta, 10), std::make_pair(ID53.delta, 11), std::make_pair(ID54.delta, 12), std::make_pair(ID55.delta, 13), std::make_pair(ID56.delta, 14), std::make_pair(ID57.delta, 15), std::make_pair(ID58.delta, 16), std::make_pair(ID59.delta, 17), std::make_pair(ID60.delta, 18), std::make_pair(ID61.delta, 19),
	std::make_pair(ID62.delta, 20), std::make_pair(ID63.delta, 21), std::make_pair(ID64.delta, 22), std::make_pair(ID65.delta, 23), std::make_pair(ID66.delta, 24), std::make_pair(ID67.delta, 25), std::make_pair(ID68.delta, 26), std::make_pair(ID69.delta, 27), std::make_pair(ID70.delta, 28), std::make_pair(ID71.delta, 29),
	std::make_pair(ID72.delta, 30), std::make_pair(ID73.delta, 31), std::make_pair(ID74.delta, 32), std::make_pair(ID75.delta, 33), std::make_pair(ID76.delta, 34), std::make_pair(ID77.delta, 35)};

#pragma endregion

#pragma region 共有データ

class Input;

class Share
{
private:
	Share() {}

	inline static std::shared_ptr<Share> instance;

	//以下に必要なデータを記載

	std::vector<Magic> casts;
	std::vector<Magic> opponentCasts;

	std::vector<Magic> learns;

	std::vector<Magic> brews;

	Inventory inventory;
	Inventory opponentInventory;

	int brewCount = 0;
	int opponentBrewCount = 0;

	int turn = -1;

	Object::Operation opponentOperation = Object::Operation::Wait;

public:
	friend Input;

	static void Create()
	{
		instance.reset(new Share());
	}

	static Share &Get()
	{
		return *instance;
	}

	//以下に必要なgetterを記載

	/**
	 * @brief ポーション生成レシピ取得
	 *
	 * @return const auto& ポーション生成レシピ
	 */
	const auto &getBrews() const { return brews; }

	/**
	 * @brief 自身の素材変換スペル取得
	 *
	 * @return const auto& 素材変換スペル
	 */
	const auto &getCasts() const { return casts; }
	/**
	 * @brief 相手の素材変換スペル取得
	 *
	 * @return const auto& 相手の素材変換スペル
	 */
	const auto &getOpponentCasts() const { return opponentCasts; }

	/**
	 * @brief 取得対象スペル一覧取得
	 *
	 * @return const auto& 取得対象スペル一覧
	 */
	const auto &getLearns() const { return learns; }

	/**
	 * @brief 自身の素材所持状態取得
	 *
	 * @return const auto& 自身の素材所持状態
	 */
	const auto &getInventory() const { return inventory; }
	/**
	 * @brief 相手の素材所持状態取得
	 *
	 * @return const auto& 相手の素材所持状態
	 */
	const auto &getOpponentInventory() const { return opponentInventory; }

	/**
	 * @brief 自身のポーション作成個数取得
	 *
	 * @return const auto 自身のポーション作成個数
	 */
	const auto getBrewCount() const { return brewCount; }
	/**
	 * @brief 相手のポーション作成個数取得
	 *
	 * @return const auto 相手のポーション作成個数
	 */
	const auto getOpponentBrewCount() const { return opponentBrewCount; }

	/**
	 * @brief 進行ターン数取得
	 *
	 * @return const auto 進行ターン数
	 */
	const auto getTurn() const { return turn; }

	/**
	 * @brief 相手の使用スペル取得
	 *
	 * @return const auto 相手の使用スペル
	 */
	const auto getOpponentOperation() const { return opponentOperation; }
};

#pragma endregion

#pragma region データ入力

class Input
{
private:
	bool eof = true;

	template <typename Type>
	Type read()
	{
		Type value;
		if (!std::cin >> value)
		{
			eof = false;
		}

#ifdef REDIRECT
		std::cerr << value << std::endl;
#endif // !REDIRECT

		return value;
	}
	template <typename Type = std::string>
	Type readLine()
	{
		Type value;
		if (!std::getline(std::cin, value))
		{
			eof = false;
		}

#ifdef REDIRECT
		std::cerr << value << std::endl;
#endif // !REDIRECT

		return value;
	}

	void ignore() const
	{
		std::cin.ignore();
	}

public:
	Input() {}

	void first()
	{
		auto &share = Share::Get();
	}

	bool loop()
	{
		auto &share = Share::Get();
		share.turn++;

		int actionCount; // the number of spells and recipes in play
		{
			std::stringstream ss(readLine());
			ss >> actionCount;
			//ignore();

			if (!eof)
				return false;
		}

		const auto opponentCastsSize = share.opponentCasts.size();

		share.casts.clear();
		share.opponentCasts.clear();
		share.learns.clear();
		share.brews.clear();
		forange(i, static_cast<size_t>(actionCount))
		{
			decltype(share.brews)::value_type magic;
			std::string actionType;

			std::stringstream ss(readLine());
			//ignore();

			int tier0, tier1, tier2, tier3;
			int castable, repeatable;

			ss >> magic.actionId // the unique ID of this spell or recipe
				>> actionType	 // CAST, OPPONENT_CAST, LEARN, BREW

				>> tier0 // tier-0 ingredient change
				>> tier1 // tier-1 ingredient change
				>> tier2 // tier-2 ingredient change
				>> tier3 // tier-3 ingredient change

				>> magic.price	   // the price in rupees if this is a potion
				>> magic.tomeIndex // the index in the tome if this is a tome spell, equal to the read-ahead tax
				>> magic.taxCount  // the amount of taxed tier-0 ingredients you gain from learning this spell
				>> castable		   // 1 if this is a castable player spell
				>> repeatable;	   // 1 if this is a repeatable player spell

			magic.delta.tier0 = static_cast<decltype(magic.delta.tier0)>(tier0);
			magic.delta.tier1 = static_cast<decltype(magic.delta.tier1)>(tier1);
			magic.delta.tier2 = static_cast<decltype(magic.delta.tier2)>(tier2);
			magic.delta.tier3 = static_cast<decltype(magic.delta.tier3)>(tier3);

			magic.castable = (0 < castable);

			magic.repeatable = (0 < repeatable);

			if (actionType == Object::RoundActionCast)
			{
				share.casts.push_back(magic);
			}
			else if (actionType == Object::RoundActionOpponentCast)
			{
				share.opponentCasts.push_back(magic);
			}
			else if (actionType == Object::RoundActionLearn)
			{
				magic.taxCount = std::min(magic.taxCount, 0x000f);
				share.learns.push_back(magic);
			}
			else if (actionType == Object::RoundActionBrew)
			{
				magic.castable = true;
				share.brews.push_back(magic);
			}
		}

		share.opponentOperation = Object::Operation::Cast;
		if (share.opponentCasts.size() != opponentCastsSize)
		{
			share.opponentOperation = Object::Operation::Learn;
		}
		// const auto brewMatch = std::count_if(share.brews.begin(),share.brews.end(),[&](const Magic& brew){
		// 	return std::any_of(bBrews.begin(), bBrews.end(), [&](const Magic& bBrew) {
		// 		return bBrew.actionId == brew.actionId;
		// 	});
		// });
		// const auto learnMatch = std::count_if(share.learns.cbegin(),share.learns.cend(),[&](const Magic& learn){
		// 	return std::any_of(bLearns.cbegin(), bLearns.cend(), [&](const Magic& bLearn) {
		// 		return bLearn.actionId == learn.actionId;
		// 	});
		// });

		{
			auto &inv = share.inventory;

			std::stringstream ss(readLine());
			//ignore();

			int tier0, tier1, tier2, tier3, price;

			ss >> tier0	  // tier-0 ingredients in inventory
				>> tier1  // tier-1 ingredients in inventory
				>> tier2  // tier-2 ingredients in inventory
				>> tier3  // tier-3 ingredients in inventory
				>> price; // amount of rupees

			inv.inv.tier0 = static_cast<decltype(inv.inv.tier0)>(tier0);
			inv.inv.tier1 = static_cast<decltype(inv.inv.tier1)>(tier1);
			inv.inv.tier2 = static_cast<decltype(inv.inv.tier2)>(tier2);
			inv.inv.tier3 = static_cast<decltype(inv.inv.tier3)>(tier3);

			if (inv.score != price)
			{
				inv.score = price;
				share.brewCount++;
			}
		}

		{
			auto &inv = share.opponentInventory;

			std::stringstream ss(readLine());
			//ignore();

			int tier0, tier1, tier2, tier3, price;

			ss >> tier0	  // tier-0 ingredients in inventory
				>> tier1  // tier-1 ingredients in inventory
				>> tier2  // tier-2 ingredients in inventory
				>> tier3  // tier-3 ingredients in inventory
				>> price; // amount of rupees

			inv.inv.tier0 = static_cast<decltype(inv.inv.tier0)>(tier0);
			inv.inv.tier1 = static_cast<decltype(inv.inv.tier1)>(tier1);
			inv.inv.tier2 = static_cast<decltype(inv.inv.tier2)>(tier2);
			inv.inv.tier3 = static_cast<decltype(inv.inv.tier3)>(tier3);

			if (inv.score != price)
			{
				inv.score = price;
				share.opponentBrewCount++;

				share.opponentOperation = Object::Operation::Brew;
			}
		}

		return true;
	}
};

#pragma endregion

#pragma region ユーティリティ

/**
 * @brief ポーション生成
 *
 * @param actionId ポーションID
 * @return std::string コマンド
 */
std::string CommandBrew(const int actionId)
{
	return Object::RoundActionBrew + " " + std::to_string(actionId);
}

/**
 * @brief スペル実行
 *
 * @param actionId スペルID
 * @return std::string コマンド
 */
std::string CommandCast(const int actionId)
{
	return Object::RoundActionCast + " " + std::to_string(actionId);
}
/**
 * @brief スペル実行
 *
 * @param actionId スペルID
 * @param times 繰返し回数
 * @return std::string コマンド
 */
std::string CommandCast(const int actionId, const int times)
{
	return CommandCast(actionId) + " " + std::to_string(times);
}

/**
 * @brief スペル取得
 *
 * @param actionId スペルID
 * @return std::string コマンド
 */
std::string CommandLearn(const int actionId)
{
	return Object::RoundActionLearn + " " + std::to_string(actionId);
}

/**
 * @brief スペル再使用
 *
 * @return std::string コマンド
 */
std::string CommandRest()
{
	return Object::RoundActionRest;
}

/**
 * @brief なにもしない
 *
 * @return std::string コマンド
 */
std::string CommandWait()
{
	return Object::RoundActionWait;
}

#pragma endregion

#pragma region AI

class CommandPack
{
private:
	Object::Operation operation = Object::Operation::Wait;
	unsigned char actionId = 0;
	unsigned char times = 1;

	CommandPack(const Object::Operation operation)
		: CommandPack(operation, 0, 0) {}
	CommandPack(const Object::Operation operation, const char actionId)
		: CommandPack(operation, actionId, 0) {}
	CommandPack(const Object::Operation operation, const char actionId, const char times)
		: operation(operation), actionId(actionId), times(times) {}

public:
	CommandPack() {}

	std::string getCommand() const
	{
		switch (operation)
		{
		case Object::Operation::Brew:
			return CommandBrew(actionId);
		case Object::Operation::Cast:
			return CommandCast(actionId, times);
		case Object::Operation::Learn:
			return CommandLearn(actionId);
		case Object::Operation::Rest:
			return CommandRest();
		case Object::Operation::Wait:
			return CommandWait();

		default:
			return CommandWait();
		}
	}

	Object::Operation getOperation() const
	{
		return operation;
	}
	int getActionId() const
	{
		return static_cast<int>(actionId);
	}

	std::tuple<Object::Operation, unsigned char, unsigned char> getParam() const
	{
		return std::make_tuple(operation, actionId, times);
	}

	std::string debugMessage() const
	{
		switch (operation)
		{
		case Object::Operation::Brew:
			return "B" + std::to_string(actionId);
		case Object::Operation::Cast:
			return "" + std::to_string(actionId) + (times == 1 ? "" : std::to_string(times));
		case Object::Operation::Learn:
			return "L" + std::to_string(actionId);
		case Object::Operation::Rest:
			return "R";
		case Object::Operation::Wait:
			return "W";

		default:
			return "-";
		}
	}

	/**
	 * @brief ポーション生成
	 *
	 * @param actionId ポーションID
	 * @return CommandPack コマンド
	 */
	static CommandPack
	Brew(const int actionId)
	{
		return CommandPack(Object::Operation::Brew, actionId);
	}

	/**
	 * @brief スペル実行
	 *
	 * @param actionId スペルID
	 * @param times 繰り返し回数
	 * @return CommandPack コマンド
	 */
	static CommandPack Cast(const int actionId, const int times = 0) { return CommandPack(Object::Operation::Cast, actionId, times); }

	/**
	 * @brief スペル取得
	 *
	 * @param actionId スペルID
	 * @return CommandPack コマンド
	 */
	static CommandPack Learn(const int actionId) { return CommandPack(Object::Operation::Learn, actionId); }

	/**
	 * @brief スペル再使用
	 *
	 * @return CommandPack コマンド
	 */
	static CommandPack Rest() { return CommandPack(Object::Operation::Rest); }

	/**
	 * @brief なにもしない
	 *
	 * @return CommandPack コマンド
	 */
	static CommandPack Wait() { return CommandPack(Object::Operation::Wait); }
};

class Simulator
{
private:
public:
};

struct MagicBit
{
	using value_type = unsigned short;

	short magic = 0;
	//0b 0000 0000 0 0000 0 0 0
	//      |    | |    | | | +Cast:Available(0x0001)
	//      |    | |    | | +Cast:Castable(0x0001 << 1)
	//      |    | |    | |
	//      |    | |    | +Brew:Available(0x0001 << 2)
	//      |    | |    |
	//      |    | |    +Brew:Index(0x000F << 3)
	//      |    | |
	//      |    | +Learn:Available(0x0001 << 7)
	//      |    |
	//      |    +Learn:Tax(0x000F << 8)
	//      |
	//      +Learn:Index(0x000F << 12)

	void setLearnTomeIndex(const int tomeIndex)
	{
		const value_type shift = 12;
		const value_type mask = 0x000F;
		const value_type smask = (mask << shift);

		const auto val = static_cast<value_type>((tomeIndex & mask) << shift);
		magic = (magic & (~smask)) | val;
	}
	int getLearnTomeIndex() const
	{
		const value_type shift = 12;
		const value_type mask = 0x000F;
		const value_type smask = (mask << shift);

		return ((magic & smask) >> shift);
	}

	void setLearnTaxCount(const int taxCount)
	{
		const value_type shift = 8;
		const value_type mask = 0x000F;
		const value_type smask = (mask << shift);

		const auto val = static_cast<value_type>((taxCount & mask) << shift);
		magic = (magic & (~smask)) | val;
	}
	int getLearnTaxCount() const
	{
		const value_type shift = 8;
		const value_type mask = 0x000F;
		const value_type smask = (mask << shift);

		return ((magic & smask) >> shift);
	}

	void setLearnAvailable(const bool available)
	{
		const value_type shift = 7;
		const value_type mask = 0x0001;
		const value_type smask = (mask << shift);

		const auto val = static_cast<value_type>(available) << shift;
		magic = (magic & (~smask)) | val;
	}
	bool getLearnAvailable() const
	{
		const value_type shift = 7;
		const value_type mask = 0x0001;
		const value_type smask = (mask << shift);

		return ((magic & smask) >> shift);
	}

	void setLearn(const int tomeIndex, const int taxCount, const bool value)
	{
		value_type val = 0;
		val = (tomeIndex & 0x000F);
		val = (val << 4) | (taxCount & 0x000F);
		val = (val << 1) | (value & 0x0001);

		magic = (magic & (~(0x03FF << 7))) | (val << 7);
	}

	void setBrewAvailable(const bool available)
	{
		const value_type shift = 2;
		const value_type mask = 0x0001;
		const value_type smask = (mask << shift);

		const auto val = static_cast<value_type>(available) << shift;
		magic = (magic & (~smask)) | val;
	}
	bool getBrewAvailable() const
	{
		const value_type shift = 2;
		const value_type mask = 0x0001;
		const value_type smask = (mask << shift);

		return ((magic & smask) >> shift);
	}

	void setBrewIndex(const int index)
	{
		const value_type shift = 3;
		const value_type mask = 0x000F;
		const value_type smask = (mask << shift);

		const auto val = static_cast<value_type>(index) << shift;
		magic = (magic & (~smask)) | val;
	}
	bool getBrewIndex() const
	{
		const value_type shift = 3;
		const value_type mask = 0x000F;
		const value_type smask = (mask << shift);

		return ((magic & smask) >> shift);
	}

	void setCastCastable(const bool castable)
	{
		const value_type shift = 1;
		const value_type mask = 0x0001;
		const value_type smask = (mask << shift);

		const auto val = static_cast<value_type>(castable) << shift;
		magic = (magic & (~smask)) | val;
	}
	bool getCastCastable() const
	{
		const value_type shift = 1;
		const value_type mask = 0x0001;
		const value_type smask = (mask << shift);

		return ((magic & smask) >> shift);
	}

	void setCastAvailable(const bool available)
	{
		const value_type shift = 0;
		const value_type mask = 0x0001;
		const value_type smask = (mask << shift);

		const auto val = static_cast<value_type>(available) << shift;
		magic = (magic & (~smask)) | val;
	}
	bool getCastAvailable() const
	{
		const value_type shift = 0;
		const value_type mask = 0x0001;
		const value_type smask = (mask << shift);

		return ((magic & smask) >> shift);
	}

	void setCast(const bool castable, const bool available)
	{
		value_type val = ((static_cast<value_type>(castable) << 1) | static_cast<value_type>(available));

		magic = (magic & (~(0x0003 << 0))) | (val << 0);
	}
	bool getCastable() const
	{
		const value_type mask = 0x0003;

		return ((magic & mask) == mask);
	}
};

class AI
{
private:
	inline static const int SearchTurn = 20;
	inline static const int ChokudaiWidth = 3;
	inline static const int SurveyTurn = 8;
	//inline static const int SurveyWidth = 5000;

	inline static const EvaluateExp<SearchTurn> evaluateExp;
	inline static const EvaluateExp<45> learnExp;
	inline static const EvaluateExp<4> learnCastExp;

	using MagicList = std::array<MagicBit, std::max(LearnSpell.size(), std::max(CastSpell.size(), BrewPostion.size()))>;

	template <size_t Length>
	struct Data
	{
		Tier inventory;
		Tier delta;
		MagicList magicList;
		std::array<CommandPack, Length> commands;
		double score = 0;
		int price = 0;
		int brewCount = 0;
		char bonus3 = 4;
		char bonus1 = 4;
	};

	using DataPack = Data<SearchTurn> *;
	using Pool = MemoryPool<Data<SearchTurn>, (1 << 18)>;
	Data<SearchTurn> topData;
	XoShiro128 xoshiro;

	struct DataLess
	{
		bool operator()(const DataPack &a, const DataPack &b) const
		{
			return a->score < b->score;
		}
	};

	using PriorityQueue = std::priority_queue<DataPack, std::vector<DataPack>, DataLess>;

	int gameTurn = 0;
	std::array<int, CastSpell.size()> convertCastActionId;
	std::array<int, BrewPostion.size()> opponentBrewTurn;

	double (AI::*evaluate)(const size_t turn, const DataPack data, const Object::Operation operation, const MagicBit magic, const size_t index);

	double evaluateMy(const size_t turn, const DataPack data, const Object::Operation operation, const MagicBit magic, const size_t index)
	{
		const double topScore = data->score;
		double score = 0;

		switch (operation)
		{
		case Object::Operation::Brew:

			if (data->brewCount < Object::PotionLimit)
			{
				const auto opponentTurn = opponentBrewTurn[index];
				if (turn <= opponentTurn)
				{
					score += data->price;
				}
				else
				{
					score += data->price;
				}
				
				//score += data->brewCount / 6.0; //早期ボーナス
			}
			else
			{
				score += data->inventory.getScore();

				//作成ターン数でできる相手のポーションの値段を加算して比較する
				//逆転されるなら破棄、勝利なら最優先で作成
				score += 100; //早期ボーナス
			}
			break;
		case Object::Operation::Cast:
			score += 1.0;
			break;
		case Object::Operation::Learn:

			//後半はあまり取得しないようにする
			score += learnExp[std::min(learnExp.size() - 1, gameTurn + turn)];

			score += (magic.getLearnTaxCount() - (magic.getLearnTomeIndex())) / 3.0;
			break;
		case Object::Operation::Rest:
			break;
		case Object::Operation::Wait:
			break;

		default:
			break;
		}

		//経過ターンによる補正
		score = (score * evaluateExp[turn]);

		//乱数によるブレ
		//score = score * SearchTurn + xoshiro.nextFloat();

		return topScore + score;
	}
	double evaluateOpponent(const size_t turn, const DataPack data, const Object::Operation operation, const MagicBit magic, const size_t index)
	{
		const double topScore = data->score;
		double score = 0;

		switch (operation)
		{
		case Object::Operation::Brew:
			score += data->price * 100;
			break;
		case Object::Operation::Cast:
			score += 1.0;
			break;
		case Object::Operation::Learn:
			score += 0.5;
			break;
		case Object::Operation::Rest:
			break;
		case Object::Operation::Wait:
			break;

		default:
			break;
		}

		return topScore + score;
	}

	Magic learn2cast(const Magic &learn) const
	{
		Magic cast = learn;

		cast.tomeIndex = -1;
		cast.taxCount = -1;
		cast.castable = true;

		cast.repeatable = LearnSpell[learn.actionId].repeatable;

		return cast;
	}

	MagicList convertInputData(const std::vector<Magic> &casts)
	{
		const auto &share = Share::Get();

		const auto &learns = share.getLearns();
		const auto &brews = share.getBrews();

		MagicList magicList;
		for (const auto &learn : learns)
		{
			const auto idx = LearnSpellMap.at(learn.delta);
			magicList[idx].setLearn(learn.tomeIndex, learn.taxCount, true);
		}

		forange(i, brews.size())
		{
			const auto idx = BrewPotionMap.at(brews[i].delta);
			magicList[idx].setBrewAvailable(true);
			magicList[idx].setBrewIndex(static_cast<int>(i));
		}

		convertCastActionId.fill(0);
		for (const auto &cast : casts)
		{
			const auto idx = CastSpellMap.at(cast.delta);
			magicList[idx].setCast(cast.castable, true);
			convertCastActionId[idx] = cast.actionId;
		}

		return magicList;
	}

	/**
	 * @brief スペル取得
	 *
	 * @param learnIndex スペル番号
	 * @param magic スペル付加情報
	 * @param turn 探査ターン
	 * @param top 探査ノード
	 * @param nextQueue 次の探査キュー
	 */
	void searchLearn(const size_t learnIndex, const MagicBit magic, const size_t turn, const DataPack top, PriorityQueue &nextQueue)
	{
		if (magic.getLearnAvailable())
		{
			if (top->inventory.tier0 >= magic.getLearnTomeIndex())
			{
				DataPack next = new (Pool::instance->get()) Data<SearchTurn>(*top);

				next->magicList[learnIndex].setCast(true, true);
				next->magicList[learnIndex].setLearnAvailable(false);
				next->delta += LearnSpellPlus[learnIndex];

				const auto index = magic.getLearnTomeIndex();
				std::for_each(next->magicList.begin(), next->magicList.end(), [index](decltype(next->magicList)::reference m) {
					if (m.getLearnAvailable())
					{
						const auto idx = m.getLearnTomeIndex();
						if (idx > index)
						{
							//インデックス減算
							m.setLearnTomeIndex(m.getLearnTomeIndex() - 1);
						}
						else
						{
							//先読み税加算
							m.setLearnTaxCount(m.getLearnTaxCount() + 1);
						}
					}
				});
				next->inventory.tier0 += std::min(Object::InventorySize - next->inventory.getSum(), magic.getLearnTaxCount() - index);

				next->commands[turn] = CommandPack::Learn(LearnSpell[learnIndex].actionId);

				next->score = (this->*evaluate)(turn, next, Object::Operation::Learn, magic, learnIndex);

				nextQueue.push(next);
			}
		}
	}
	/**
	 * @brief ポーション生成
	 *
	 * @param potionIndex スペル番号
	 * @param magic スペル付加情報
	 * @param turn 探査ターン
	 * @param top 探査ノード
	 * @param nextQueue 次の探査キュー
	 */
	void searchBrew(const size_t potionIndex, const MagicBit magic, const size_t turn, const DataPack top, PriorityQueue &nextQueue, PriorityQueue &lastQueue)
	{
		if (magic.getBrewAvailable())
		{
			if (top->inventory.isAccept(BrewPostion[potionIndex].delta))
			{
				DataPack next = new (Pool::instance->get()) Data<SearchTurn>(*top);

				next->magicList[potionIndex].setBrewAvailable(false);
				const int index = next->magicList[potionIndex].getBrewIndex();
				std::for_each(next->magicList.begin(), next->magicList.end(), [index](decltype(next->magicList)::reference m) {
					if (m.getLearnAvailable())
					{
						const auto idx = m.getLearnTomeIndex();
						if (idx > index)
						{
							//インデックス減算
							m.setBrewIndex(m.getBrewIndex() - 1);
						}
					}
				});

				next->inventory += BrewPostion[potionIndex].delta;
				next->delta += LearnSpellPlus[potionIndex];

				const int bonus = [&]() {
					if (index == 0 && next->bonus3 > 0)
					{
						next->bonus3--;
						return 3;
					}
					else if (index == 0 && next->bonus1 > 0)
					{
						next->bonus1--;
						return 1;
					}
					else if (index == 1 && next->bonus1 > 0)
					{
						next->bonus1--;
						return 1;
					}
					else
						return 0;
				}();

				next->price += BrewPostion[potionIndex].price + bonus;
				next->brewCount += 1;

				next->commands[turn] = CommandPack::Brew(BrewPostion[potionIndex].actionId);

				next->score = (this->*evaluate)(turn, next, Object::Operation::Brew, magic, potionIndex);

				if (next->brewCount < Object::PotionLimit)
				{
					nextQueue.push(next);
				}
				else
				{
					lastQueue.push(next);
				}
			}
		}
	}
	/**
	 * @brief 素材変換
	 *
	 * @param castIndex スペル番号
	 * @param magic スペル付加情報
	 * @param turn 探査ターン
	 * @param top 探査ノード
	 * @param nextQueue 次の探査キュー
	 */
	void searchCast(const size_t castIndex, const MagicBit magic, const size_t turn, const DataPack top, PriorityQueue &nextQueue)
	{
		if (magic.getCastable())
		{
			if (top->inventory.isAccept(CastSpell[castIndex].delta))
			{
				DataPack next = new (Pool::instance->get()) Data<SearchTurn>(*top);

				next->magicList[castIndex].setCastCastable(false);

				next->inventory += CastSpell[castIndex].delta;

				next->commands[turn] = CommandPack::Cast(convertCastActionId[CastSpell[castIndex].actionId], 1);

				next->score = (this->*evaluate)(turn, next, Object::Operation::Cast, magic, castIndex);

				nextQueue.push(next);

				if (CastSpell[castIndex].repeatable)
				{
					int times = 2;
					auto inv = next->inventory;
					while (inv.isAccept(CastSpell[castIndex].delta))
					{
						DataPack next2 = new (Pool::instance->get()) Data<SearchTurn>(*next);

						inv += CastSpell[castIndex].delta;
						next->inventory = inv;

						next->commands[turn] = CommandPack::Cast(convertCastActionId[CastSpell[castIndex].actionId], times);

						nextQueue.push(next);

						times++;
					}
				}
			}
		}
	}
	/**
	 * @brief 素材変換
	 * 
	 * @param castIndex スペル番号
	 * @param times スペル繰返回数
	 * @param magic スペル付加情報
	 * @param turn 探査ターン
	 * @param top 探査ノード
	 * @param nextQueue 次の探査キュー
	 */
	void searchCast(const size_t castIndex, const int times, const MagicBit magic, const size_t turn, const DataPack top, PriorityQueue &nextQueue)
	{
		if (magic.getCastable())
		{
			if (top->inventory.isAccept(CastSpell[castIndex].delta))
			{
				DataPack next = new (Pool::instance->get()) Data<SearchTurn>(*top);

				next->magicList[castIndex].setCastCastable(false);

				forange(t, times)
				{
					next->inventory += CastSpell[castIndex].delta;
				}
				next->commands[turn] = CommandPack::Cast(convertCastActionId[CastSpell[castIndex].actionId], times);

				next->score = (this->*evaluate)(turn, next, Object::Operation::Cast, magic, castIndex);

				nextQueue.push(next);
			}
		}
	}
	/**
	 * @brief 休憩
	 *
	 * @param castIndex スペル番号
	 * @param magic スペル付加情報
	 * @param turn 探査ターン
	 * @param top 探査ノード
	 * @param nextQueue 次の探査キュー
	 */
	void searchRest(const size_t turn, const DataPack top, PriorityQueue &nextQueue)
	{
		DataPack next = new (Pool::instance->get()) Data<SearchTurn>(*top);

		for (auto &magic : next->magicList)
		{
			if (magic.getCastAvailable())
				magic.setCastCastable(true);
		}

		next->commands[turn] = CommandPack::Rest();

		next->score = (this->*evaluate)(turn, next, Object::Operation::Rest, MagicBit{}, 0);

		nextQueue.push(next);
	}

	/**
	 * @brief 前回の最善手をセットする
	 *
	 * @param chokudaiSearch
	 */
	void setLastCommand(std::array<PriorityQueue, SearchTurn + 1> &chokudaiSearch)
	{
		forange(turn, SearchTurn - 1)
		{
			if (chokudaiSearch[turn].empty())
				break;

			const auto top = chokudaiSearch[turn].top();
			chokudaiSearch[turn].pop();

			const auto [ope, id, times] = topData.commands[turn + 1].getParam();

			switch (ope)
			{
			case Object::Operation::Brew:
			{
				const auto idx = id - BrewPostion.front().actionId;
				searchBrew(idx, top->magicList[idx], turn, top, chokudaiSearch[turn + 1], chokudaiSearch[SearchTurn]);
			}
			break;

			case Object::Operation::Cast:
			{

				const auto idx = [&]() {
					forange(i, convertCastActionId.size())
					{
						if (convertCastActionId[i] == id)
							return i;
					}
					return static_cast<size_t>(0);
				}();
				searchCast(idx, times, top->magicList[idx], turn, top, chokudaiSearch[turn + 1]);
			}
			break;

			case Object::Operation::Learn:
			{
				const auto idx = id;
				searchLearn(idx, top->magicList[idx], turn, top, chokudaiSearch[turn + 1]);
			}
			break;

			case Object::Operation::Rest:
				searchRest(turn, top, chokudaiSearch[turn + 1]);
				break;

			default:
				break;
			}

			Pool::instance->release(top);
		}
	}

	void thinkOpponent()
	{
		const auto &share = Share::Get();

		std::array<PriorityQueue, SearchTurn + 1> chokudaiSearch;
		{
			DataPack init = new (Pool::instance->get()) Data<SearchTurn>();

			init->inventory = share.getOpponentInventory().inv;

			init->magicList = convertInputData(share.getOpponentCasts());

			//init->brewCount = share.getOpponentBrewCount();
			//終了判定をさせない
			init->brewCount = 0;
			init->price = share.getOpponentInventory().score;

			const auto brews = share.getBrews();
			init->bonus3 = brews[0].taxCount;
			init->bonus1 = brews[1].taxCount;

			evaluate = &AI::evaluateOpponent;

			chokudaiSearch.front().push(init);
		}

		opponentBrewTurn.fill(std::numeric_limits<int>::max());

		MilliSecTimer timer(std::chrono::milliseconds(10));
		timer.start();
		while (!timer)
		{
			forange(turn, SurveyTurn)
			{
				forange(w, ChokudaiWidth)
				{
					if (chokudaiSearch[turn].empty())
						break;

					const auto top = chokudaiSearch[turn].top();
					chokudaiSearch[turn].pop();

					if (turn > 0 && top->commands[turn - 1].getOperation() == Object::Operation::Brew)
					{
						const auto idx = top->commands[turn - 1].getActionId() - BrewPostion.front().actionId;
						opponentBrewTurn[idx] = std::min(opponentBrewTurn[idx], static_cast<int>(turn - 1));
					}
					else
					{
						forange(i, top->magicList.size())
						{
							const auto &magic = top->magicList[i];

							searchBrew(i, magic, turn, top, chokudaiSearch[turn + 1], chokudaiSearch[SearchTurn]);
							searchLearn(i, magic, turn, top, chokudaiSearch[turn + 1]);
							searchCast(i, magic, turn, top, chokudaiSearch[turn + 1]);
						}

						searchRest(turn, top, chokudaiSearch[turn + 1]);
					}

					Pool::instance->release(top);
				}
			}
		}
	}

public:
	AI()
	{
		Pool::Create();
	}

	std::string think()
	{
		Pool::instance->clear();

		const auto &share = Share::Get();
		gameTurn = share.getTurn();

		{
			thinkOpponent();
			Pool::instance->clear();
		}

		std::array<PriorityQueue, SearchTurn + 1> chokudaiSearch;
		{
			DataPack init = new (Pool::instance->get()) Data<SearchTurn>();

			init->inventory = share.getInventory().inv;
			//init->delta = LearnSpellPlus[ID78.actionId] + LearnSpellPlus[ID79.actionId] + LearnSpellPlus[ID80.actionId] + LearnSpellPlus[ID81.actionId];

			init->magicList = convertInputData(share.getCasts());

			init->brewCount = share.getBrewCount();
			init->price = share.getInventory().score;

			const auto brews = share.getBrews();
			init->bonus3 = brews[0].taxCount;
			init->bonus1 = brews[1].taxCount;

			evaluate = &AI::evaluateMy;

			if (share.getOpponentOperation() == Object::Operation::Cast)
			{
				DataPack init2 = new (Pool::instance->get()) Data<SearchTurn>(*init);

				chokudaiSearch.front().push(init);
				setLastCommand(chokudaiSearch);
				chokudaiSearch.front().push(init2);
			}
			else
			{
				chokudaiSearch.front().push(init);
			}
		}

		MilliSecTimer timer(std::chrono::milliseconds(35));

		int loopCount = 0;

		timer.start();
		while (!timer)
		{
			loopCount++;
			forange(turn, SearchTurn)
			{
				forange(w, ChokudaiWidth)
				{
					if (chokudaiSearch[turn].empty())
						break;

					const auto top = chokudaiSearch[turn].top();
					chokudaiSearch[turn].pop();

					forange(i, top->magicList.size())
					{
						const auto &magic = top->magicList[i];

						searchBrew(i, magic, turn, top, chokudaiSearch[turn + 1], chokudaiSearch[SearchTurn]);
						searchLearn(i, magic, turn, top, chokudaiSearch[turn + 1]);
						searchCast(i, magic, turn, top, chokudaiSearch[turn + 1]);
					}

					searchRest(turn, top, chokudaiSearch[turn + 1]);

					Pool::instance->release(top);
				}
			}
		}

		if (chokudaiSearch.back().empty())
		{
			errerLine("想定外のエラー");
			return CommandRest();
		}
		else
		{
			topData = *chokudaiSearch.back().top();
			const auto com = topData.commands.front().getCommand();

			std::string debugMes = "";

			int setTurn = 0;
			forange(i, topData.commands.size())
			{
				if (topData.commands[i].getOperation() == Object::Operation::Brew)
				{
					debugMes += topData.commands[i].debugMessage() + "@" + std::to_string(setTurn) + " ";
				}
				setTurn++;
			}

			debugMes += "P" + std::to_string(topData.price) + "," + std::to_string(loopCount);
			debugMes = "";
			forange(i, opponentBrewTurn.size())
			{
				if (opponentBrewTurn[i] != std::numeric_limits<int>::max())
				{
					debugMes += std::to_string(i + BrewPostion.front().actionId) + "@" + std::to_string(opponentBrewTurn[i]) + " ";
				}
			}

			return com + " " + debugMes;
		}
	}
};

#pragma endregion

#pragma region データ出力

int main()
{

	Share::Create();

	Input input;
	input.first();

	Stopwatch sw;

	AI ai;

	while (input.loop())
	{
		sw.start();
		const auto &coms = ai.think();
		sw.stop();

		errerLine(sw.toString_ms());

		std::cout << coms << " " << sw.toString_ms() << std::endl;
	}

	return 0;
}

#pragma endregion
