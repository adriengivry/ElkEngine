#include "ElkTools/Utils/Profiler.h"
#include "ElkTools/Debug/Log.h"
#include "ElkTools/Utils/IniLoader.h"

#include <iomanip>

using namespace ElkTools::Debug;

bool ElkTools::Utils::Profiler::__RUNNING = false;
std::unordered_map<std::string, double> ElkTools::Utils::Profiler::__ELPASED_HISTORY;
std::unordered_map<std::string, uint64_t> ElkTools::Utils::Profiler::__CALLS_COUNTER;

void ElkTools::Utils::Profiler::Start()
{
	__RUNNING = true;
	m_logFrequency = static_cast<double>(Utils::IniLoader::GetFloat("profiler.ini", "log_frequency"));
	m_currentTime = std::chrono::high_resolution_clock::now();
	m_timer = std::chrono::duration<double>(0.0);
	__ELPASED_HISTORY.clear();
	Debug::Log::Process("Profiler : ON", ElkTools::Debug::Log::LogLevel::LOG_INFO);
}

void ElkTools::Utils::Profiler::Stop()
{
	__RUNNING = false;
	__ELPASED_HISTORY.clear();
	Debug::Log::Process("Profiler : OFF", ElkTools::Debug::Log::LogLevel::LOG_INFO);
}

void ElkTools::Utils::Profiler::Toggle()
{
	if (__RUNNING)	Stop();
	else			Start();
}

void ElkTools::Utils::Profiler::Log()
{
	if (__RUNNING)
	{
		m_lastTime = m_currentTime;
		m_currentTime = std::chrono::high_resolution_clock::now();
		m_elapsed = m_currentTime - m_lastTime;

		m_timer += m_elapsed;

		if (m_timer.count() > 1.0)
		{
			m_timer = std::chrono::duration<double>(0.0);

			ElkTools::Debug::Log::Process("---- [Profiler Stats] ----");

			m_sortedHistory.clear();

			for (auto& data : __ELPASED_HISTORY)
			{
				m_sortedHistory.insert(std::pair<double, std::string>(data.second, data.first));
			}

			for (auto& data : m_sortedHistory)
			{
				std::string action = data.second;
				std::string duration = std::to_string(data.first) + " sec.";
				std::string percentage = std::to_string((data.first / m_elapsed.count()) * 100.0) + "%";
				std::string calls = std::to_string(__CALLS_COUNTER[data.second]) + " calls";
				ElkTools::Debug::Log::Process("METHOD:" + action, Log::LogLevel::LOG_DEFAULT, Log::LogColor::BLUE);
				ElkTools::Debug::Log::Process("    TOTAL DURATION:" + duration);
				ElkTools::Debug::Log::Process("    FRAME PERCENTAGE:" + percentage);
				ElkTools::Debug::Log::Process("    CALLS COUNTER:" + calls);
				ElkTools::Debug::Log::LineBreak();
			}
		}

		__ELPASED_HISTORY.clear();
		__CALLS_COUNTER.clear();
	}
}

void ElkTools::Utils::Profiler::Save(Spy & p_spy)
{
	if (__ELPASED_HISTORY.find(p_spy.name) != __ELPASED_HISTORY.end())
	{
		__ELPASED_HISTORY[p_spy.name] += std::chrono::duration<double>(p_spy.end - p_spy.start).count();
	}
	else
	{
		__ELPASED_HISTORY[p_spy.name] = std::chrono::duration<double>(p_spy.end - p_spy.start).count();
	}

	if (__CALLS_COUNTER.find(p_spy.name) != __CALLS_COUNTER.end())
	{
		++__CALLS_COUNTER[p_spy.name];
	}
	else
	{
		__CALLS_COUNTER[p_spy.name] = 1;
	}
}
