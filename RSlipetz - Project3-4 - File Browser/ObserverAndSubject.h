#if !defined(FILE_BROWSER_MODEL)
#define FILE_BROWSER_MODEL
#include <set>
#include <map>
#include <filesystem>

class Subject;

class Observer {
public:
	virtual void Update() = 0;
};

class Subject {
protected:
	std::set<std::shared_ptr<Observer*>> _observerSet;
public:
	void Attach(std::shared_ptr<Observer*> const& obs) { _observerSet.insert(obs); }
	void Detach(std::shared_ptr<Observer*> const& obs) { _observerSet.erase(obs); }
	virtual void UpdateObservers() = 0;
};
#endif