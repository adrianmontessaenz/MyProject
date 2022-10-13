#pragma once

namespace Engine
{
	class RunTime
	{
	public:
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void LateUpdate() = 0;
		virtual void Shutdown() = 0;

		void SetShutdown(const bool shutdown);
		bool IsShutdown() const;

		void SetEnabled(const bool enable);
		bool IsEnabled() const;
	protected:
		RunTime() = default;

	private:
		bool mIsEnabled;
		bool mIsShutdown;
	};
}