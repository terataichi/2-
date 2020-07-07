#pragma once
#include <memory>

//namespace single
//{
//	template<class T>
//	class Singleton
//	{
//	public:
//		typedef T InstanceType;
//
//		static InstanceType& GetInstance()
//		{
//			return *_sInstance;
//		}
//
//
//	private:
//		// カスタムデリーター
//		struct SingletonDeleter
//		{
//			void operator()(InstanceType* singleton)const
//			{
//				delete& singleton;
//			}
//		};
//
//		static std::unique_ptr<InstanceType, SingletonDeleter> _sInstance;	//
//
//		Singleton();
//		virtual ~Singleton() {};
//	};
//
//	template<class T>
//	typename Singleton<T>::InstanceType* Singleton<T>::_sInstance;