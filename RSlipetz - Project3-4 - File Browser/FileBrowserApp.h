#if !defined(FILE_BROWSER)
#define FILE_BROWSER
#include "ConsoleApp.h"


class FileBrowser : public ConsoleApp {
	friend int main(int, char*[]);
public:
	FileBrowser(bool const& isRecursive = false, std::string const& regex = R"rgx(.*)rgx", std::string filepath = "C:\\");
	~FileBrowser() { }
	
#pragma region InnerClasses

	class FileBrowserModel : public Subject {
		std::string _regex;
		std::string _filePath;
		bool _isRecursive;
		std::vector<std::string> _results;
		long long _filesSearched;
		long long _filesMatched;
		long long _fileSize;
		std::string _tabs;
		bool _regexError;
	public:
		FileBrowserModel(bool const& recursive = false, std::string const& regex = R"rgx(.*)rgx", std::string const& filePath = "C:\\") : _isRecursive(recursive), _regex(regex), _filePath(filePath), _tabs("") { RefreshModel(); }

		void SetRecursive(bool const& recursive) { _isRecursive = recursive; }
		void SetRegex(std::string const& regex) { _regex = std::string(regex); }
		void SetFilePath(std::string const& filePath) { _filePath = filePath; }

		bool GetRecursive() { return _isRecursive; }
		std::string GetRegex() { return _regex; }
		std::string GetFilepath() { return _filePath; }

		std::vector<std::string> GetModelResults() const { return _results; }
		long long GetNumSearched() const { return _filesSearched; }
		long long GetNumMatched() const { return _filesMatched; }
		long long GetFileSizes() const { return _fileSize; }

		void RefreshModel();

		void UpdateObservers() {
			for (std::shared_ptr<Observer*> obs : _observerSet) {
				(*obs)->Update();
			}
		}
	private:
		void PerformRecursiveSearch(std::tr2::sys::path const& pathToSearch);
		bool FileMatchesRegex(std::tr2::sys::path const& pathToMatch);
	};

	class ModelStatsWindow : public Window, public Observer {
		std::shared_ptr<FileBrowser*> _controllerHook;
	public:
		ModelStatsWindow(FileBrowser* const& hook);
		~ModelStatsWindow() {}
		
		void RouteMouseEvent(MouseEvent const& me) override { return; }
		void RouteKeyEvent(KeyEvent const& ke) override { return; }
		void Update() override;
	};

	class ModelChangeWindow : public Window, public Observer {
		std::shared_ptr<FileBrowser*> _controllerHook;
	public:
		ModelChangeWindow(FileBrowser* const& hook);
		~ModelChangeWindow() {}
		void RouteMouseEvent(MouseEvent const& me) override;
		void RouteKeyEvent(KeyEvent const& ke) override;
		void Update() override;
	};

	class ModelDisplayWindow : public Window, public Observer {
		std::shared_ptr<FileBrowser*> _controllerHook;
		std::vector<std::string> _modelResults;
	public:
		ModelDisplayWindow(FileBrowser* const& hook);
		~ModelDisplayWindow() {}

		void RouteMouseEvent(MouseEvent const& me) override;
		void RouteKeyEvent(KeyEvent const& ke) override;
		void Update() override;
	};

#pragma endregion
};








#endif