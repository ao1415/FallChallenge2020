#pragma GCC optimize "O3,omit-frame-pointer,inline"

#include <iostream>

#include <string>

#include <vector>
#include <array>
#include <set>
#include <map>
#include <queue>

#include <chrono>
#include <algorithm>
#include <memory>

#pragma region ライブラリ

#define forange(counter, end) forstep(counter, 0, end)
#define forstep(counter, begin, end) forstep_type(size_t, counter, begin, end)
#define forange_type(type, counter, end) forstep_type(type, counter, 0, end)
#define forstep_type(type, counter, begin, end) for (type counter = begin, forstep_type_end_##counter = end; counter < forstep_type_end_##counter; counter++)

/// <summary>
/// 時間計測を行うクラス
/// </summary>
class Stopwatch
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Stopwatch() = default;

	/// <summary>
	/// 計測を開始する
	/// </summary>
	inline void start() noexcept
	{
		s = std::chrono::high_resolution_clock::now();
		e = s;
	}
	/// <summary>
	/// 計測を停止する
	/// </summary>
	inline void stop() noexcept
	{
		e = std::chrono::high_resolution_clock::now();
	}

	/// <summary>
	/// 計測時間を取得する(ナノ秒)
	/// </summary>
	/// <returns>計測時間(ナノ秒)</returns>
	inline const long long nanoseconds() const noexcept { return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count(); }
	/// <summary>
	/// 計測時間を取得する(マイクロ秒)
	/// </summary>
	/// <returns>計測時間(マイクロ秒)</returns>
	inline const long long microseconds() const noexcept { return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count(); }
	/// <summary>
	/// 計測時間を取得する(ミリ秒)
	/// </summary>
	/// <returns>計測時間(ミリ秒)</returns>
	inline const long long millisecond() const noexcept { return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count(); }

	/// <summary>
	/// 単位付きの計測時間の文字列を得る(ナノ秒)
	/// </summary>
	/// <returns>計測時間の文字列(ナノ秒)</returns>
	inline const std::string toString_ns() const { return std::to_string(nanoseconds()) + "ns"; }
	/// <summary>
	/// 単位付きの計測時間の文字列を得る(マイクロ秒)
	/// </summary>
	/// <returns>計測時間の文字列(マイクロ秒)</returns>
	inline const std::string toString_us() const { return std::to_string(microseconds()) + "us"; }
	/// <summary>
	/// 単位付きの計測時間の文字列を得る(ミリ秒)
	/// </summary>
	/// <returns>計測時間の文字列(ミリ秒)</returns>
	inline const std::string toString_ms() const { return std::to_string(millisecond()) + "ms"; }

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	std::chrono::time_point<std::chrono::high_resolution_clock> e;
};

/// <summary>
/// 一定時間の経過を確認するクラス
/// </summary>
class MilliSecTimer
{
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	unsigned long long int startCycle = 0;
	long long time = 0;

	const double CyclePerMilliSec = 2794000.0;

#ifndef _MSC_VER
	unsigned long long int getCycle() const
	{
		unsigned int low, high;
		__asm__ volatile("rdtsc"
						 : "=a"(low), "=d"(high));
		return ((unsigned long long int)low) | ((unsigned long long int)high << 32);
	}
#endif // _MSC_VER

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	MilliSecTimer() = default;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(ミリ秒)</param>
	MilliSecTimer(const std::chrono::milliseconds &_time) noexcept { time = _time.count(); }

	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(ミリ秒)</param>
	void set(const std::chrono::milliseconds &_time) noexcept { time = _time.count(); }

	/// <summary>
	/// タイマーを開始させる
	/// </summary>
	void start() noexcept
	{
#ifdef _MSC_VER
		s = std::chrono::high_resolution_clock::now();
#else
		startCycle = getCycle();
#endif // _MSC_VER
	}

	/// <summary>
	/// 設定時間経過したかを得る
	/// </summary>
	/// <returns>経過していれば true, それ以外は false</returns>
	inline const bool check() const noexcept
	{
#ifdef _MSC_VER
		const auto e = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count() >= time;
#else
		return (getCycle() - startCycle) / CyclePerMilliSec >= time;
#endif // _MSC_VER
	}

	/// <summary>
	/// 設定時間経過したかを得る
	/// </summary>
	/// <returns>経過していれば true, それ以外は false</returns>
	operator bool() const noexcept { return check(); }

	/// <summary>
	/// 経過時間を取得する(ミリ秒)
	/// </summary>
	/// <returns>計測時間(ミリ秒)</returns>
	inline const long long interval() const noexcept
	{
#ifdef _MSC_VER
		const auto e = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count();
#else
		return static_cast<long long int>((getCycle() - startCycle) / CyclePerMilliSec);
#endif // _MSC_VER
	}
};

#pragma endregion

#pragma region 定数宣言

namespace Object
{
	const std::string RoundActionCast = "CAST";
	const std::string RoundActionRest = "REST";
	const std::string RoundActionBrew = "BREW";
	const std::string RoundActionWait = "WAIT";

} // namespace Object

#pragma endregion

#pragma region データ構造

struct Tier
{
	int 
};


struct Magic
{
	int actionId = 0;		 // the unique ID of this spell or recipe
	std::string actionType{};	 // in the first league: BREW; later: CAST, OPPONENT_CAST, LEARN, BREW
	int delta0 = 0;			 // tier-0 ingredient change
	int delta1 = 0;			 // tier-1 ingredient change
	int delta2 = 0;			 // tier-2 ingredient change
	int delta3 = 0;			 // tier-3 ingredient change
	int price = 0;			 // the price in rupees if this is a potion
	int tomeIndex = 0;		 // in the first two leagues: always 0; later: the index in the tome if this is a tome spell, equal to the read-ahead tax
	int taxCount = 0;		 // in the first two leagues: always 0; later: the amount of taxed tier-0 ingredients you gain from learning this spell
	bool castable = false;	 // in the first league: always 0; later: 1 if this is a castable player spell
	bool repeatable = false; // for the first two leagues: always 0; later: 1 if this is a repeatable player spell
};

struct Inventory
{
	int inv0 = 0;  // tier-0 ingredients in inventory
	int inv1 = 0;  // tier-1 ingredients in inventory
	int inv2 = 0;  // tier-2 ingredients in inventory
	int inv3 = 0;  // tier-3 ingredients in inventory
	int score = 0; // amount of rupees
};

#pragma endregion

#pragma region 共有データ

class Input;

class Share
{
private:
	Share() {}

	inline static std::shared_ptr<Share> instance;

	//以下に必要なデータを記載

	std::vector<Magic> brews;
	std::vector<Inventory> inventories;

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

	const auto &getBrews() const { return brews; }
	const auto &getInventories() const { return inventories; }
};

#pragma endregion

#pragma region データ入力

#define REDIRECT

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
		actionCount = read<decltype(actionCount)>();
		ignore();

		share.brews.clear();
		forange(i, static_cast<size_t>(actionCount))
		{
			decltype(share.brews)::value_type magic;

			magic.actionId = read<decltype(magic.actionId)>();	   // the unique ID of this spell or recipe
			magic.actionType = read<decltype(magic.actionType)>(); // in the first league: BREW; later: CAST, OPPONENT_CAST, LEARN, BREW
			magic.delta0 = read<decltype(magic.delta0)>();		   // tier-0 ingredient change
			magic.delta1 = read<decltype(magic.delta1)>();		   // tier-1 ingredient change
			magic.delta2 = read<decltype(magic.delta2)>();		   // tier-2 ingredient change
			magic.delta3 = read<decltype(magic.delta3)>();		   // tier-3 ingredient change
			magic.price = read<decltype(magic.price)>();		   // the price in rupees if this is a potion
			magic.tomeIndex = read<decltype(magic.tomeIndex)>();   // in the first two leagues: always 0; later: the index in the tome if this is a tome spell, equal to the read-ahead tax
			magic.taxCount = read<decltype(magic.taxCount)>();	   // in the first two leagues: always 0; later: the amount of taxed tier-0 ingredients you gain from learning this spell
			magic.castable = read<decltype(magic.castable)>();	   // in the first league: always 0; later: 1 if this is a castable player spell
			magic.repeatable = read<decltype(magic.repeatable)>(); // for the first two leagues: always 0; later: 1 if this is a repeatable player spell

			ignore();

			if (magic.actionType==Object::RoundActionBrew)
			{
				share.brews.push_back(magic);
			}
		}

		share.inventories.clear();
		forange(i, 2)
		{
			decltype(share.inventories)::value_type inv;

			inv.inv0 = read<decltype(inv.inv0)>();	 // tier-0 ingredients in inventory
			inv.inv1 = read<decltype(inv.inv1)>();	 // tier-1 ingredients in inventory
			inv.inv2 = read<decltype(inv.inv2)>();	 // tier-2 ingredients in inventory
			inv.inv3 = read<decltype(inv.inv3)>();	 // tier-3 ingredients in inventory
			inv.score = read<decltype(inv.score)>(); // amount of rupees

			ignore();
		}
	}
};

#pragma endregion

#pragma region ユーティリティ

/// <summary>
/// ポーション生成
/// </summary>
/// <param name="actionId">ポーションID</param>
/// <returns></returns>
std::string CommandBrew(const int actionId){
	return Object::RoundActionBrew + " " + std::to_string(id);
}

/// <summary>
/// スペル実行
/// </summary>
/// <param name="actionId">スペルID</param>
/// <returns></returns>
std::string CommandCast(const int actionId){
	return Object::RoundActionCast + " " + std::to_string(id);
}

#pragma endregion

#pragma region AI

struct Data
{
};

class Simulator
{
private:
public:
};

class AI
{
public:
	std::string think()
	{
		const auto &share = Share::Get();

		const auto &brews = share.getBrews();
		
		const auto maxBrewIte = max_element(brews.cbegin(), brews.cend(), [](const auto &a, const auto &b) {
			return a.price < b.price;
		});

		return CommandBrew(maxBrewIte->actionId);
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

		std::cout << coms << std::endl;
	}

	return 0;
}

#pragma endregion
