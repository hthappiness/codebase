
namespace Doreamon{
	namespace base{
		
		// feel the happiness and logic when creating a class
		template<typename callable>
		class CResGuard
		{
		public:
			
			CResGuard(callable F)
			{
				m_func = F;
			}
			~CResGuard()
			{
				if(bFinish == false)
				{
					m_func();
				}
			}
			
			CResGuard(CResGuard&) = delete;
			CResGuard& operator=(CResGuard&) = delete ;
			
			void markFinish()
			{
				bFinish = true;
			}
		private:
			bool bFinish{false};
			callable m_func;
		};
		
		static void testGuard(int num)
		{
			ULONG ulHndl = createHndl();
			CResGuard guard([&]{
				ReleaseHndl(handle);
			});
			
			if( num % 2)
			{
				return ;
			}
			
			guard.markFinish();
			
			return ;
		}
	};
};