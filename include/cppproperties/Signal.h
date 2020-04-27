#pragma once
#include <unordered_map>
#include <unordered_set>
#include <type_traits>
#include <typeindex>
#include <functional>

namespace ps
{
	//###########################################################################
	//#
	//#                        Signal                            
	//#
	//############################################################################


	//pointer to member function utilities
	template<typename>
	struct PMF_traits
	{
		using member_type = void;
		using class_type = void;
	};

	template<class T, class U>
	struct PMF_traits<U T::*>
	{
		using member_type = U;
		using class_type = T;
	};

	//specialized signal implementation suited to our needs
	//it handles class ptr + PMF (with or without argument)
	//it handles callable funtors (with or without argument)
	//the underlying storage uses std::function<void()>
	//and a specialized lambda funtion to abstract the different cases
	//another requirement is that you can't add the same PMF twice
	class Signal
	{
	protected:
		std::unordered_map<std::type_index, std::function<void(const void*)>> m_slots;
	public:
		Signal() = default;

		//// connects a member function with an argument of type T to this signal
		template <typename T, typename classT, typename pmfT>
		std::type_index connect(classT *inst, pmfT&& func) noexcept
		{
			using PMF = PMF_traits<pmfT>;
			static_assert(std::is_same_v<classT, std::decay_t<typename PMF::class_type>>, "Member func ptr type has to match instance type.");
			if constexpr (std::is_same_v<T, void>)
			{
				m_slots.try_emplace(std::type_index(typeid(pmfT)), [inst, func](const void*)
				{
					(inst->*func)();
				});
			}
			else
			{
				m_slots.try_emplace(std::type_index(typeid(pmfT)), [inst, func](const void* valPtr)
				{
					(inst->*func)(*static_cast<const T*>(valPtr));
				});
			}

			return std::type_index(typeid(pmfT));
		}


		// connects a callable function with an argument of type T to the signal
		template<typename T, typename FuncT>
		std::type_index connect(FuncT&& func) noexcept
		{
			if constexpr (std::is_same_v<T, void>)
			{
				m_slots.try_emplace(std::type_index(typeid(FuncT)), [func = std::forward<FuncT>(func)](const void*)
				{
					func();
				});
			}
			else
			{
				m_slots.try_emplace(std::type_index(typeid(FuncT)), [func = std::forward<FuncT>(func)](const void* valPtr)
				{
					func(*static_cast<const T*>(valPtr));
				});
			}

			return std::type_index(typeid(FuncT));
		}

		bool empty() const noexcept
		{
			return m_slots.empty();
		}

		// disconnects all previously connected functions
		void disconnect()
		{
			m_slots.clear();
		}
		//disconnects the function with the given type index
		void disconnect(std::type_index idx)
		{
			m_slots.erase(idx);
		}

		// calls all connected functions
		void emit(const void* value) const
		{
			for (auto& [typeID, slot] : m_slots)
				slot(value);
		}


		void emitUnique(const void* value, std::unordered_set<std::type_index>& alreadyInvoked)
		{
			for (auto& [typeID, slot] : m_slots)
				if (auto it = alreadyInvoked.find(typeID); it == alreadyInvoked.end())
				{
					slot(value);
					alreadyInvoked.emplace_hint(it, typeID);
				}		
		}
	};
}
