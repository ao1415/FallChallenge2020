#pragma GCC optimize "O3,omit-frame-pointer,inline"

#define REDIRECT_

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

/**
 * @brief ���Ԍv�����s���N���X
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
	 * @brief �v�����J�n����
	 *
	 */
	inline void start() noexcept
	{
		s = std::chrono::high_resolution_clock::now();
		e = s;
	}
	/**
	 * @brief �v�����~����
	 *
	 */
	inline void stop() noexcept
	{
		e = std::chrono::high_resolution_clock::now();
	}

	/**
	 * @brief �v�����Ԃ��擾����(�i�m�b)
	 *
	 * @return const long long �v������(�i�m�b)
	 */
	inline const long long nanoseconds() const noexcept { return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count(); }
	/**
	 * @brief �v�����Ԃ��擾����(�}�C�N���b)
	 *
	 * @return const long long �v������(�}�C�N���b)
	 */
	inline const long long microseconds() const noexcept { return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count(); }
	/**
	 * @brief �v�����Ԃ��擾����(�~���b)
	 *
	 * @return const long long �v������(�~���b)
	 */
	inline const long long millisecond() const noexcept { return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count(); }

	/**
	 * @brief �P�ʕt���̌v�����Ԃ̕�����𓾂�(�i�m�b)
	 *
	 * @return const std::string �v�����Ԃ̕�����(�i�m�b)
	 */
	inline const std::string toString_ns() const { return std::to_string(nanoseconds()) + "ns"; }
	/**
	 * @brief �P�ʕt���̌v�����Ԃ̕�����𓾂�(�}�C�N���b)
	 *
	 * @return const std::string �v�����Ԃ̕�����(�}�C�N���b)
	 */
	inline const std::string toString_us() const { return std::to_string(microseconds()) + "us"; }
	/**
	 * @brief �P�ʕt���̌v�����Ԃ̕�����𓾂�(�~���b)
	 *
	 * @return const std::string �v�����Ԃ̕�����(�~���b)
	 */
	inline const std::string toString_ms() const { return std::to_string(millisecond()) + "ms"; }

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	std::chrono::time_point<std::chrono::high_resolution_clock> e;
};

/**
 * @brief ��莞�Ԃ̌o�߂��m�F����N���X
 *
 */
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
	/**
	 * @brief Construct a new Milli Sec Timer object
	 *
	 */
	MilliSecTimer() = default;
	/**
	 * @brief Construct a new Milli Sec Timer object
	 *
	 * @param _time �ݒ莞��(�~���b)
	 */
	MilliSecTimer(const std::chrono::milliseconds &_time) noexcept { time = _time.count(); }

	/**
	 * @brief ���Ԃ�ݒ肷��
	 *
	 * @param _time �ݒ莞��(�~���b)
	 */
	void set(const std::chrono::milliseconds &_time) noexcept { time = _time.count(); }

	/**
	 * @brief �^�C�}�[���J�n������
	 *
	 */
	void start() noexcept
	{
#ifdef _MSC_VER
		s = std::chrono::high_resolution_clock::now();
#else
		startCycle = getCycle();
#endif // _MSC_VER
	}

	/**
	 * @brief �ݒ莞�Ԍo�߂������𓾂�
	 *
	 * @return true �ݒ莞�Ԃ��o�߂��Ă���
	 * @return false �ݒ莞�Ԉȓ�
	 */
	inline const bool check() const noexcept
	{
#ifdef _MSC_VER
		const auto e = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count() >= time;
#else
		return (getCycle() - startCycle) / CyclePerMilliSec >= time;
#endif // _MSC_VER
	}

	/**
	 * @brief �ݒ莞�Ԍo�߂������𓾂�
	 *
	 * @return true �ݒ莞�Ԃ��o�߂��Ă���
	 * @return false �ݒ莞�Ԉȓ�
	 */
	operator bool() const noexcept { return check(); }

	/**
	 * @brief �o�ߎ��Ԃ��擾����(�~���b)
	 *
	 * @return const long long �o�ߎ���(�~���b)
	 */
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
	const std::string RoundActionOpponentCast = "OPPONENT_CAST ";
	const std::string RoundActionLearn = "LEARN";
	const std::string RoundActionBrew = "BREW";
	const std::string RoundActionRest = "REST";
	const std::string RoundActionWait = "WAIT";

} // namespace Object

#pragma endregion

#pragma region �f�[�^�\��

struct Tier
{
	using value_type = int;

	value_type tier0 = value_type();
	value_type tier1 = value_type();
	value_type tier2 = value_type();
	value_type tier3 = value_type();

	Tier() {}
	Tier(value_type tier0, value_type tier1, value_type tier2, value_type tier3)
		: tier0(tier0),
		  tier1(tier1),
		  tier2(tier2),
		  tier3(tier3) {}

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

	bool isAccept(const Tier &o) const
	{
		bool isOver = true;
		isOver &= (0 <= tier0 + o.tier0);
		isOver &= (0 <= tier1 + o.tier1);
		isOver &= (0 <= tier2 + o.tier2);
		isOver &= (0 <= tier3 + o.tier3);

		return isOver;
	}
};

struct Magic
{
	int actionId = 0;		  // the unique ID of this spell or recipe
	std::string actionType{}; // in the first league: CAST, OPPONENT_CAST, LEARN, BREW
	Tier delta;				  // tier ingredient change
	int price = 0;			  // the price in rupees if this is a potion
	int tomeIndex = 0;		  // the index in the tome if this is a tome spell, equal to the read-ahead tax
	int taxCount = 0;		  // the amount of taxed tier-0 ingredients you gain from learning this spell
	bool castable = false;	  // in the first league: always 0; later: 1 if this is a castable player spell
	bool repeatable = false;  // for the first two leagues: always 0; later: 1 if this is a repeatable player spell
};

struct Inventory
{
	Tier inv;	   // tier ingredient change
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

	std::vector<Magic> casts;
	std::vector<Magic> opponentCasts;

	std::vector<Magic> learns;

	std::vector<Magic> brews;

	Inventory inventory;
	Inventory opponentInventory;

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

	/**
	 * @brief �|�[�V�����������V�s�擾
	 *
	 * @return const auto& �|�[�V�����������V�s
	 */
	const auto &getBrews() const { return brews; }

	/**
	 * @brief ���g�̑f�ޕϊ��X�y���擾
	 *
	 * @return const auto& �f�ޕϊ��X�y��
	 */
	const auto &getCasts() const { return casts; }
	/**
	 * @brief ����̑f�ޕϊ��X�y���擾
	 *
	 * @return const auto& ����̑f�ޕϊ��X�y��
	 */
	const auto &getOpponentCasts() const { return opponentCasts; }

	/**
	 * @brief �擾�ΏۃX�y���ꗗ�擾
	 *
	 * @return const auto& �擾�ΏۃX�y���ꗗ
	 */
	const auto &getLearns() const { return learns; }

	/**
	 * @brief ���g�̑f�ޏ�����Ԏ擾
	 *
	 * @return const auto& ���g�̑f�ޏ������
	 */
	const auto &getInventory() const { return inventory; }
	/**
	 * @brief ����̑f�ޏ�����Ԏ擾
	 *
	 * @return const auto& ����̑f�ޏ������
	 */
	const auto &getOpponentInventory() const { return opponentInventory; }
};

#pragma endregion

#pragma region �f�[�^����

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

		share.casts.clear();
		share.opponentCasts.clear();
		share.learns.clear();
		share.brews.clear();
		forange(i, static_cast<size_t>(actionCount))
		{
			decltype(share.brews)::value_type magic;

			magic.actionId = read<decltype(magic.actionId)>();		 // the unique ID of this spell or recipe
			magic.actionType = read<decltype(magic.actionType)>();	 // CAST, OPPONENT_CAST, LEARN, BREW
			magic.delta.tier0 = read<decltype(magic.delta.tier0)>(); // tier-0 ingredient change
			magic.delta.tier1 = read<decltype(magic.delta.tier1)>(); // tier-1 ingredient change
			magic.delta.tier2 = read<decltype(magic.delta.tier2)>(); // tier-2 ingredient change
			magic.delta.tier3 = read<decltype(magic.delta.tier3)>(); // tier-3 ingredient change
			magic.price = read<decltype(magic.price)>();			 // the price in rupees if this is a potion
			magic.tomeIndex = read<decltype(magic.tomeIndex)>();	 // the index in the tome if this is a tome spell, equal to the read-ahead tax
			magic.taxCount = read<decltype(magic.taxCount)>();		 // the amount of taxed tier-0 ingredients you gain from learning this spell

			int castable = read<decltype(castable)>();
			magic.castable = (0 < castable); // 1 if this is a castable player spell

			int repeatable = read<decltype(repeatable)>();
			magic.repeatable = (0 < repeatable); // 1 if this is a repeatable player spell

			ignore();

			if (magic.actionType == Object::RoundActionCast)
			{
				share.casts.push_back(magic);
			}
			else if (magic.actionType == Object::RoundActionOpponentCast)
			{
				share.opponentCasts.push_back(magic);
			}
			else if (magic.actionType == Object::RoundActionLearn)
			{
				share.learns.push_back(magic);
			}
			else if (magic.actionType == Object::RoundActionBrew)
			{
				magic.castable = true;
				share.brews.push_back(magic);
			}
		}

		{
			auto &inv = share.inventory;

			inv.inv.tier0 = read<decltype(inv.inv.tier0)>(); // tier-0 ingredients in inventory
			inv.inv.tier1 = read<decltype(inv.inv.tier1)>(); // tier-1 ingredients in inventory
			inv.inv.tier2 = read<decltype(inv.inv.tier2)>(); // tier-2 ingredients in inventory
			inv.inv.tier3 = read<decltype(inv.inv.tier3)>(); // tier-3 ingredients in inventory
			inv.score = read<decltype(inv.score)>();		 // amount of rupees

			ignore();
		}

		{
			auto &inv = share.opponentInventory;

			inv.inv.tier0 = read<decltype(inv.inv.tier0)>(); // tier-0 ingredients in inventory
			inv.inv.tier1 = read<decltype(inv.inv.tier1)>(); // tier-1 ingredients in inventory
			inv.inv.tier2 = read<decltype(inv.inv.tier2)>(); // tier-2 ingredients in inventory
			inv.inv.tier3 = read<decltype(inv.inv.tier3)>(); // tier-3 ingredients in inventory
			inv.score = read<decltype(inv.score)>();		 // amount of rupees

			ignore();
		}
	}
};

#pragma endregion

#pragma region ���[�e�B���e�B

/**
 * @brief �|�[�V��������
 *
 * @param actionId �|�[�V����ID
 * @return std::string �R�}���h
 */
std::string CommandBrew(const int actionId)
{
	return Object::RoundActionBrew + " " + std::to_string(actionId);
}

/**
 * @brief �X�y�����s
 *
 * @param actionId �X�y��ID
 * @return std::string �R�}���h
 */
std::string CommandCast(const int actionId)
{
	return Object::RoundActionCast + " " + std::to_string(actionId);
}
/**
 * @brief �X�y�����s
 *
 * @param actionId �X�y��ID
 * @param times �J�Ԃ���
 * @return std::string �R�}���h
 */
std::string CommandCast(const int actionId, const int times)
{
	return CommandCast(actionId) + " " + std::to_string(times);
}

/**
 * @brief �X�y���Ďg�p
 *
 * @return std::string �R�}���h
 */
std::string CommandRest()
{
	return Object::RoundActionRest;
}

/**
 * @brief �X�y���Ďg�p
 *
 * @return std::string �R�}���h
 */
std::string CommandWait()
{
	return Object::RoundActionWait;
}

#pragma endregion

#pragma region AI

template <size_t Length>
struct Data
{
	Tier inventory;
	std::vector<Magic> brews;
	std::vector<Magic> casts;
	std::array<std::string, Length> commands;
	int count = 0;
	int score = 0;
	int castCount = 0;

	bool operator<(const Data<Length> &o) const
	{
		if (count != o.count)
			return count < o.count;

		if (score != o.score)
			return score < o.score;

		return castCount < o.castCount;
	}
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

		const auto &inv = share.getInventory();

		const auto &brews = share.getBrews();
		const auto &casts = share.getCasts();

		const int Turn = 10;
		const int Width = 50;
		using DataPack = std::shared_ptr<Data<Turn>>;

		struct DataLess
		{
			bool operator()(const DataPack &a, const DataPack &b) const
			{
				if (a->count != b->count)
					return a->count < b->count;

				if (a->score != b->score)
					return a->score < b->score;

				return a->castCount < b->castCount;
			}
		};

		std::array<std::priority_queue<DataPack, std::vector<DataPack>, DataLess>, Turn + 1> chokudaiSearch;
		{
			DataPack init = std::make_shared<DataPack::element_type>();

			init->inventory = inv.inv;
			init->brews = brews;
			init->casts = casts;

			chokudaiSearch.front().push(init);
		}

		forange(turn, Turn)
		{
			forange(w, Width)
			{
				if (chokudaiSearch[turn].empty())
					continue;

				const auto top = chokudaiSearch[turn].top();
				chokudaiSearch[turn].pop();

				for (auto &brew : top->brews)
				{
					if (brew.castable)
					{
						if (top->inventory.isAccept(brew.delta))
						{
							DataPack next = std::make_shared<DataPack::element_type>();

							brew.castable = false;
							(*next) = (*top);
							brew.castable = true;

							next->inventory += brew.delta;

							next->count += 1;
							next->score += brew.price;

							next->commands[turn] = CommandBrew(brew.actionId);

							chokudaiSearch[turn + 1].push(next);
						}
					}
				}

				for (auto &cast : top->casts)
				{
					if (cast.castable)
					{
						if (top->inventory.isAccept(cast.delta))
						{
							DataPack next = std::make_shared<DataPack::element_type>();

							cast.castable = false;
							(*next) = (*top);
							cast.castable = true;

							next->inventory += cast.delta;

							next->castCount += 1;

							next->commands[turn] = CommandCast(cast.actionId);

							chokudaiSearch[turn + 1].push(next);
						}
					}
				}

				{
					DataPack next = std::make_shared<DataPack::element_type>();

					(*next) = (*top);

					for (auto &cast : next->casts)
					{
						cast.castable = true;
					}

					next->commands[turn] = CommandRest();

					chokudaiSearch[turn + 1].push(next);
				}
			}
		}

		if (chokudaiSearch.back().empty())
		{
			std::cerr << "�z��O�̃G���[" << std::endl;
			return CommandRest();
		}
		else
		{
			return chokudaiSearch.back().top()->commands.front();
		}
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
