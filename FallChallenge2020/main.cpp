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

		std::cerr << "Memory Size:" << memorySize / 1024.0 / 1024.0 << "MB" << std::endl;
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
				std::cerr << "MemoryPool out of range" << std::endl;
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
			m_data[x] = exp(-static_cast<double>(x) / static_cast<double>(Size * 2));
		}
	}

	[[nodiscard]] double operator[](const size_t index) const
	{
		return m_data[index];
	}
};

#pragma endregion

#pragma region 定数宣言

namespace Object
{
	const std::string RoundActionCast = "CAST";
	const std::string RoundActionOpponentCast = "OPPONENT_CAST ";
	const std::string RoundActionLearn = "LEARN";
	const std::string RoundActionBrew = "BREW";
	const std::string RoundActionRest = "REST";
	const std::string RoundActionWait = "WAIT";

	const int InventorySize = 10;

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

	value_type getSum() const
	{
		return tier0 + tier1 + tier2 + tier3;
	}

	value_type getScore() const
	{
		return tier1 + tier2 + tier3;
	}

	Tier operator+(const Tier &o) const
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

	bool operator==(const Tier &o) const
	{
		return (tier0 == o.tier0 && tier1 == o.tier1 && tier2 == o.tier2 && tier3 == o.tier3);
	}

	bool operator<(const Tier &o) const
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
	 * @return const auto& 自身のポーション作成個数
	 */
	const auto &getBrewCount() const { return brewCount; }
	/**
	 * @brief 相手のポーション作成個数取得
	 *
	 * @return const auto& 相手のポーション作成個数
	 */
	const auto &getOpponentBrewCount() const { return opponentBrewCount; }
};

#pragma endregion

#pragma region データ入力

class Input
{
private:
	template <typename Type>
	Type read() const
	{
		Type value;
		std::cin >> value;

#ifdef REDIRECT
		std::cerr << value << std::endl;
#endif // !REDIRECT

		return value;
	}
	template <typename Type = std::string>
	Type readLine() const
	{
		Type value;
		std::getline(std::cin, value);

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

	void loop()
	{
		auto &share = Share::Get();

		int actionCount; // the number of spells and recipes in play
		{
			std::stringstream ss(readLine());
			ss >> actionCount;
			//ignore();
		}

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
			}
		}
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
	char actionId = 0;
	char times = 0;

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
			if (times > 0)
				return CommandCast(actionId, times);
			else
				return CommandCast(actionId);
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
		return actionId;
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
	//0b 0000 0000 00 00 00 00
	//      |    | || || || |+Cast:Available(0x0001)
	//      |    | || || || +Cast:Castable(0x0001 << 1)
	//      |    | || || |+Brew:Available(0x0001 << 2)
	//      |    | || || +Brew:None
	//      |    | || ++None:None
	//      |    | |+Learn:Available(0x0001 << 6)
	//      |    | +Learn:None
	//      |    +Learn:Tax(0x000F << 8)
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
		const value_type shift = 6;
		const value_type mask = 0x0001;
		const value_type smask = (mask << shift);

		const auto val = static_cast<value_type>(available) << shift;
		magic = (magic & (~smask)) | val;
	}
	bool getLearnAvailable() const
	{
		const value_type shift = 6;
		const value_type mask = 0x0001;
		const value_type smask = (mask << shift);

		return ((magic & smask) >> shift);
	}

	void setLearn(const int tomeIndex, const int taxCount, const bool value)
	{
		value_type val = 0;
		val = (tomeIndex & 0x000F);
		val = (val << 4) | (taxCount & 0x000F);
		val = (val << 2) | (value & 0x0001);

		magic = (magic & (~(0x03FF << 6))) | (val << 6);
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
	inline static const int SurveyTurn = 3;

	inline static const EvaluateExp<SearchTurn> evaluateExp;

	using MagicList = std::array<MagicBit, std::max(LearnSpell.size(), std::max(CastSpell.size(), BrewPostion.size()))>;

	template <size_t Length>
	struct Data
	{
		Tier inventory;
		MagicList magicList;
		std::array<CommandPack, Length> commands;
		double score = 0;
		int price = 0;
		int brewCount = 0;
	};

	using DataPack = Data<SearchTurn> *;
	using Pool = MemoryPool<Data<SearchTurn>, (1 << 18)>;

	struct DataLess
	{
		bool operator()(const DataPack &a, const DataPack &b) const
		{
			return a->score < b->score;
		}
	};

	using PriorityQueue = std::priority_queue<DataPack, std::vector<DataPack>, DataLess>;

	std::array<int, CastSpell.size()> convertCastActionId;

	double evaluate(const size_t turn, const DataPack data, const Object::Operation operation, const size_t index) const
	{
		const double topScore = data->score;
		double score;

		switch (operation)
		{
		case Object::Operation::Brew:
			score += BrewPostion[index].price * 1000.0;
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

		return topScore + score * evaluateExp[turn];
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
		for (const auto &brew : brews)
		{
			const auto idx = BrewPotionMap.at(brew.delta);
			magicList[idx].setBrewAvailable(true);
		}
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
	 * @param chokudaiSearch 探査キュー配列
	 */
	void searchLearn(const size_t learnIndex, const MagicBit magic, const size_t turn, const DataPack top, std::array<PriorityQueue, SearchTurn + 1> &chokudaiSearch) const
	{
		if (magic.getLearnAvailable())
		{
			if (top->inventory.tier0 >= magic.getLearnTomeIndex())
			{
				DataPack next = new (Pool::instance->get()) Data<SearchTurn>(*top);

				next->magicList[learnIndex].setCast(true, true);
				next->magicList[learnIndex].setLearnAvailable(false);

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

				next->score = evaluate(turn, next, Object::Operation::Learn, learnIndex);

				chokudaiSearch[turn + 1].push(next);
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
	 * @param chokudaiSearch 探査キュー配列
	 */
	void searchBrew(const size_t potionIndex, const MagicBit magic, const size_t turn, const DataPack top, std::array<PriorityQueue, SearchTurn + 1> &chokudaiSearch) const
	{
		if (magic.getBrewAvailable())
		{
			if (top->inventory.isAccept(BrewPostion[potionIndex].delta))
			{
				DataPack next = new (Pool::instance->get()) Data<SearchTurn>(*top);

				next->magicList[potionIndex].setBrewAvailable(false);

				next->inventory += BrewPostion[potionIndex].delta;

				next->price += BrewPostion[potionIndex].price;
				next->brewCount += 1;

				next->commands[turn] = CommandPack::Brew(BrewPostion[potionIndex].actionId);

				next->score = evaluate(turn, next, Object::Operation::Brew, potionIndex);

				chokudaiSearch[turn + 1].push(next);
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
	 * @param chokudaiSearch 探査キュー配列
	 */
	void searchCast(const size_t castIndex, const MagicBit magic, const size_t turn, const DataPack top, std::array<PriorityQueue, SearchTurn + 1> &chokudaiSearch) const
	{
		if (magic.getCastable())
		{
			if (top->inventory.isAccept(CastSpell[castIndex].delta))
			{
				DataPack next = new (Pool::instance->get()) Data<SearchTurn>(*top);

				next->magicList[castIndex].setCastCastable(false);

				next->inventory += CastSpell[castIndex].delta;

				next->commands[turn] = CommandPack::Cast(convertCastActionId[CastSpell[castIndex].actionId]);

				next->score = evaluate(turn, next, Object::Operation::Cast, castIndex);

				chokudaiSearch[turn + 1].push(next);

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

						chokudaiSearch[turn + 1].push(next);

						times++;
					}
				}
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
	 * @param chokudaiSearch 探査キュー配列
	 */
	void searchRest(const size_t turn, const DataPack top, std::array<PriorityQueue, SearchTurn + 1> &chokudaiSearch) const
	{
		DataPack next = new (Pool::instance->get()) Data<SearchTurn>(*top);

		for (auto &magic : next->magicList)
		{
			if (magic.getCastAvailable())
				magic.setCastCastable(true);
		}

		next->commands[turn] = CommandPack::Rest();
	
		next->score = evaluate(turn, next, Object::Operation::Rest, 0);
		
		chokudaiSearch[turn + 1].push(next);
	}

	void setLearn(std::array<PriorityQueue, SearchTurn + 1> &chokudaiSearch) const
	{
		forange(turn, 3)
		{
			while (!chokudaiSearch[turn].empty())
			{
				const auto top = chokudaiSearch[turn].top();
				chokudaiSearch[turn].pop();

				forange(i, top->magicList.size())
				{
					const auto &magic = top->magicList[i];

					//スペル取得
					searchLearn(i, magic, turn, top, chokudaiSearch);

					//素材変換
					searchCast(i, magic, turn, top, chokudaiSearch);
				}
				
				//休憩
				searchRest(turn, top, chokudaiSearch);
				
				Pool::instance->release(top);
			}
		}
	}

	std::tuple<int, int, std::array<size_t, BrewPostion.size()>, std::array<CommandPack, SearchTurn>>
	getPreliminarySurvey(const bool opponent, std::array<PriorityQueue, SearchTurn + 1> &chokudaiSearch)
	{
		const auto &share = Share::Get();

		{
			DataPack init = new (Pool::instance->get()) Data<SearchTurn>();

			if (!opponent)
			{
				init->inventory = share.getInventory().inv;
				init->magicList = convertInputData(share.getCasts());
			}
			else
			{
				init->inventory = share.getOpponentInventory().inv;
				init->magicList = convertInputData(share.getOpponentCasts());
			}

			chokudaiSearch.front().push(init);
		}
		const int brewCount = (!opponent) ? share.getBrewCount() : share.getOpponentBrewCount();

		std::array<size_t, BrewPostion.size()> brewble;
		brewble.fill(std::numeric_limits<decltype(brewble)::value_type>::max());
		const int EndScore = 100000;

		forange(turn, SurveyTurn)
		{
			while (!chokudaiSearch[turn].empty())
			{
				const auto top = chokudaiSearch[turn].top();
				chokudaiSearch[turn].pop();

				switch (top->commands[turn].getOperation())
				{
				case Object::Operation::Brew:
				{

					//最短作成ターン
					const int brewId = top->commands[turn].getActionId() - ID42.actionId;
					brewble[brewId] = std::min(brewble[brewId], turn);

					if (top->brewCount + brewCount >= 6)
					{
						//最大終了スコア
						top->score += (top->price + top->inventory.getScore()) * EndScore;
					}
					break;
				}
				case Object::Operation::Learn:
					break;

				default:
					break;
				}

				forange(i, top->magicList.size())
				{
					const auto &magic = top->magicList[i];

					searchLearn(i, magic, turn, top, chokudaiSearch);
					searchBrew(i, magic, turn, top, chokudaiSearch);
					searchCast(i, magic, turn, top, chokudaiSearch);
				}

				searchRest(turn, top, chokudaiSearch);

				Pool::instance->release(top);
			}
		}

		if (chokudaiSearch[SurveyTurn].empty())
		{
			std::array<CommandPack, SearchTurn> commands;
			return std::make_tuple(-1, -1, brewble, commands);
		}
		else
		{
			const auto top = chokudaiSearch[SurveyTurn].top();
			int price = static_cast<int>(top->score / EndScore);

			return std::make_tuple(price, top->price + top->inventory.getScore(), brewble, top->commands);
		}
	}

public:
	AI()
	{
		Pool::Create();
		convertCastActionId.fill(-1);
	}

	std::string think()
	{
		Pool::instance->clear();

		const auto &share = Share::Get();

		// {
		// 	std::array<PriorityQueue, SearchTurn + 1> search;
		// 	std::array<PriorityQueue, SearchTurn + 1> opponentsearch;
		// 	const auto [price, nowPrice, brewble, commands] = getPreliminarySurvey(false, search);
		// 	const auto [opponentPrice, nowOpponentPrice, opponentBrewble, opponentCommands] = getPreliminarySurvey(true, opponentsearch);

		// 	//いろいろ考え直し
		// 	if ((price > 0 && price > nowOpponentPrice) || (opponentPrice > 0 && nowPrice > opponentPrice))
		// 	{
		// 		return commands.front().getCommand() + " 勝ち申した";
		// 	}
		// 	else
		// 	{
		// 	}
		// }

		std::array<PriorityQueue, SearchTurn + 1> chokudaiSearch;
		{
			DataPack init = new (Pool::instance->get()) Data<SearchTurn>();

			init->inventory = share.getInventory().inv;
			init->magicList = convertInputData(share.getCasts());

			chokudaiSearch.front().push(init);
		}

		MilliSecTimer timer(std::chrono::milliseconds(20));

		int loopCount = 0;

		timer.start();
		while (!timer)
		{
			forange(turn, SearchTurn)
			{
				forange(w, ChokudaiWidth)
				{
					if (chokudaiSearch[turn].empty())
						break;
					loopCount++;
					const auto top = chokudaiSearch[turn].top();
					chokudaiSearch[turn].pop();

					forange(i, top->magicList.size())
					{
						const auto &magic = top->magicList[i];

						searchBrew(i, magic, turn, top, chokudaiSearch);
						searchLearn(i, magic, turn, top, chokudaiSearch);
						searchCast(i, magic, turn, top, chokudaiSearch);
					}

					searchRest(turn, top, chokudaiSearch);

					Pool::instance->release(top);
				}
			}
		}

		if (chokudaiSearch.back().empty())
		{
			std::cerr << "想定外のエラー" << std::endl;
			return CommandRest();
		}
		else
		{
			const auto top = chokudaiSearch.back().top();
			const auto com = top->commands.front().getCommand();
			const auto mes = "loop:" + std::to_string(loopCount);

			return com + " " + mes;
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

	while (true)
	{
		input.loop();

		sw.start();
		const auto &coms = ai.think();
		sw.stop();

		std::cerr << sw.toString_ms() << std::endl;

		std::cout << coms << " " << sw.toString_ms() << std::endl;
	}

	return 0;
}

#pragma endregion
