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

#pragma region ���C�u����

#define forange(counter, end) forstep(counter, 0, end)
#define forstep(counter, begin, end) forstep_type(size_t, counter, begin, end)
#define forange_type(type, counter, end) forstep_type(type, counter, 0, end)
#define forstep_type(type, counter, begin, end) for (type counter = begin, forstep_type_end_##counter = end; counter < forstep_type_end_##counter; counter++)

/// <summary>
/// ���Ԍv�����s���N���X
/// </summary>
class Stopwatch
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Stopwatch() = default;

	/// <summary>
	/// �v�����J�n����
	/// </summary>
	inline void start() noexcept
	{
		s = std::chrono::high_resolution_clock::now();
		e = s;
	}
	/// <summary>
	/// �v�����~����
	/// </summary>
	inline void stop() noexcept
	{
		e = std::chrono::high_resolution_clock::now();
	}

	/// <summary>
	/// �v�����Ԃ��擾����(�i�m�b)
	/// </summary>
	/// <returns>�v������(�i�m�b)</returns>
	inline const long long nanoseconds() const noexcept { return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count(); }
	/// <summary>
	/// �v�����Ԃ��擾����(�}�C�N���b)
	/// </summary>
	/// <returns>�v������(�}�C�N���b)</returns>
	inline const long long microseconds() const noexcept { return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count(); }
	/// <summary>
	/// �v�����Ԃ��擾����(�~���b)
	/// </summary>
	/// <returns>�v������(�~���b)</returns>
	inline const long long millisecond() const noexcept { return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count(); }

	/// <summary>
	/// �P�ʕt���̌v�����Ԃ̕�����𓾂�(�i�m�b)
	/// </summary>
	/// <returns>�v�����Ԃ̕�����(�i�m�b)</returns>
	inline const std::string toString_ns() const { return std::to_string(nanoseconds()) + "ns"; }
	/// <summary>
	/// �P�ʕt���̌v�����Ԃ̕�����𓾂�(�}�C�N���b)
	/// </summary>
	/// <returns>�v�����Ԃ̕�����(�}�C�N���b)</returns>
	inline const std::string toString_us() const { return std::to_string(microseconds()) + "us"; }
	/// <summary>
	/// �P�ʕt���̌v�����Ԃ̕�����𓾂�(�~���b)
	/// </summary>
	/// <returns>�v�����Ԃ̕�����(�~���b)</returns>
	inline const std::string toString_ms() const { return std::to_string(millisecond()) + "ms"; }

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	std::chrono::time_point<std::chrono::high_resolution_clock> e;
};

/// <summary>
/// ��莞�Ԃ̌o�߂��m�F����N���X
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
	/// �R���X�g���N�^
	/// </summary>
	MilliSecTimer() = default;
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_time">�ݒ莞��(�~���b)</param>
	MilliSecTimer(const std::chrono::milliseconds &_time) noexcept { time = _time.count(); }

	/// <summary>
	/// ���Ԃ�ݒ肷��
	/// </summary>
	/// <param name="_time">�ݒ莞��(�~���b)</param>
	void set(const std::chrono::milliseconds &_time) noexcept { time = _time.count(); }

	/// <summary>
	/// �^�C�}�[���J�n������
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
	/// �ݒ莞�Ԍo�߂������𓾂�
	/// </summary>
	/// <returns>�o�߂��Ă���� true, ����ȊO�� false</returns>
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
	/// �ݒ莞�Ԍo�߂������𓾂�
	/// </summary>
	/// <returns>�o�߂��Ă���� true, ����ȊO�� false</returns>
	operator bool() const noexcept { return check(); }

	/// <summary>
	/// �o�ߎ��Ԃ��擾����(�~���b)
	/// </summary>
	/// <returns>�v������(�~���b)</returns>
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

#pragma region �萔�錾

namespace Object
{
	const std::string RoundActionCast = "CAST";
	const std::string RoundActionRest = "REST";
	const std::string RoundActionBrew = "BREW";
	const std::string RoundActionWait = "WAIT";

} // namespace Object

#pragma endregion

#pragma region �f�[�^�\��

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

#pragma region ���L�f�[�^

class Input;

class Share
{
private:
	Share() {}

	inline static std::shared_ptr<Share> instance;

	//�ȉ��ɕK�v�ȃf�[�^���L��

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

	//�ȉ��ɕK�v��getter���L��

	const auto &getBrews() const { return brews; }
	const auto &getInventories() const { return inventories; }
};

#pragma endregion

#pragma region �f�[�^����

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

#pragma region ���[�e�B���e�B

/// <summary>
/// �|�[�V��������
/// </summary>
/// <param name="actionId">�|�[�V����ID</param>
/// <returns></returns>
std::string CommandBrew(const int actionId){
	return Object::RoundActionBrew + " " + std::to_string(id);
}

/// <summary>
/// �X�y�����s
/// </summary>
/// <param name="actionId">�X�y��ID</param>
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

#pragma region �f�[�^�o��

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
