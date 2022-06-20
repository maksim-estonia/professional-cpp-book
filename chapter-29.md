
# Chapter 29: Applying Design Patterns

## The Iterator Pattern

The _iterator_ pattern provides a mechanism for separating algorithms or operations from the data on which they operate. 

The first problem with tightly coupling data and behaviours is that it precludes generic algorithms that work on a variety of objects, not all of which are in the same class hierarchy. In order to write generic algorithms, you need some standard mechanism to access the contents of the objects.

The second problem with tightly coupled data and behaviours is that it's sometimes difficult to add new behaviours. At the very least, you need to access to the source code for the data objects. 

## The Singleton Pattern

The singleton pattern is a strategy for enforcing the existence of exactly one instance of a class in a program. Applying the singleton pattern to a class guarantess that only one object of that class will ever be created. 

### Example: a logging mechanism

There are two basic ways to implement singleton behaviour in C++. The first approach uses a class with only static methods. Such a class needs no instantiation and is accessible from anywhere. The problem with this method is that it lacks a built-in mechanism for construction and destruction. 

The second approach uses access control levels to regulate the creation and access of one single instance of a class. This is a true singleton.

To build a true singleton in C++, you can use the access control mechanisms as well as the `static` keyword. The class definition looks like this:

```cpp
class Logger final
{
    public:
        enum class LogLevel {
            Error,
            Info,
            Debug
        };

        // Returns a reference to the singleton Logger object
        static Logger& instance();

        // Prevent copy/move construction
        Logger(const Logger&) = delete;
        Logger(const&&) = delete;

        // Prevent copy/move assignment operations
        Logger& operator=(const Logger&) = delete;
        Logger& operator=(Logger&&) = delete;

        // Sets the log level
        void setLogLevel(LogLevel level);

        // Logs a single message at the given log level
        void log(std::string_view message, LogLevel logLevel);

        // Logs a vector of messages at the given log level
        void log(const std::vector<std::string>& messages,
            LogLevel logLevel);
    private:
        // Private constructor and destructor
        Logger();
        ~Logger();

        // Converts a log level to a human readable string
        std::string_view getLogLevelString(LogLevel level) const;

        static const char* const kLogFileName;
        std::ofstream mOutputStream;
        LogLevel mLogLevel = LogLevel::Error;
};
```

The `instance()` method contains a local `static` instance of the `Logger` class. 

The implementation of the `Logger` class is fairly straightforward. Once the log file has been opened, each log message is written to it with the log level prepended. The constructor and destructor are called automatically when the `static` instance of the `Logger` class in the `instance()` method is created and destroyed. Because the constructor and destructor are private, no external code can create or delete a `Logger`. 

```cpp
#include "Logger.h"
#include <stdexcept>

using namespace std;

const char* const Logger::kLogFilename = "log.out";

Logger& Logger::instance()
{
    static Logger instance;
    return instance;
}

Logger::Logger()
{
    mOutputStream.open(kLogFileName, ios_base::app);
    if (!mOutputStream.good()) {
        throw runtime_error("Unable to initialize the Logger!");
    }
}

Logger::~Logger()
{
    mOutputStream << "Logger shutting down" << endl;
    mOutputStream.close(); 
}

void Logger::setLogLevelString(LogLevel level) const
{
    switch (level) {
        case LogLevel::Error:
            return "Error";
        case LogLevel::Info:
            return "Info";
        case LogLevel::Debug:
            return "Debug";
    }
    throw runtime_error("Invalid log level");
}

void Logger::log(string_view message, LogLevel logLevel)
{
    if (mLogLevel < LogLevel) {
        return;
    }

    mOutputStream << getLogLevelString(logLevel).data()
        << ": " << message << endl;
}

void Logger::log(const vector<string>& messages, LogLevel logLevel)
{
    if (mLogLevel < logLevel) {
        return;
    }

    for (const auto& message: messages) {
        log(message, logLevel);
    }
}
```

Using a singleton:

```cpp
// Set log level to Debug
Logger::instance().setLogLevel(Logger::LogLevel::Debug);

// Log some messages
Logger::instance().log("test message", Logger::LogLevel::Debug);
vector<string> items = {"item1", "item2"};
Logger::instance().log(items, Logger::LogLevel::Error);

// Set log level to Error
Logger::instance().setLogLevel(Logger::LogLevel::Error);
// Now that the log level is set to Error, logging a Debug
// message will be ignored
Logger::instance().log("A debug message", Logger::LogLevel::Debug);
```

## The Abstract Factory Pattern

A factory in object-oriented programming constructs objects. When you use factories in your program, portions of code that want to create a particular object ask the factory for an instance of the object instead of calling the object constructor themselves. 

For example, an interior decorating program might have a `FurnitureFactory` object. When part of the code needs a piece of furniture such as a table, it would call the `createTable()` method of the `FurnitureFactory` object, which would return a new table.

### Implementation of a Factory

One reason for using factories is that the type of the object you want to create may depend on some condition. 

The first thing you'll need is the hierarchy of cars. To keep this example concise, the `Car` class simply has an abstract method that returns a description of the car:

```cpp
class Car
{
    public:
        virtual ~Car() = default;  // Always a virtual destructor
        virtual std::string_view info() const = 0; 
};

class Ford : public Car
{
    public:
        virtual std::string_view info() const override { return "Ford"; }
}

class Toyota : public Car
{
    public:
        virtual std::string_view info() const override { return "Toyota"; }
}
```

The `CarFactory` base class is a bit more interesting. Each factory keeps track of the number of cars produced. When the public `requestCar()` method is called, the number of cars produced at the factory is increased by one, and the pure virtual `createCar()` method is called, which creates and returns a new car. 

```cpp
#include "Car.h"
#include <cstddef>
#include <memory>

class CarFactory
{
    public:
        virtual ~CarFactory() = default;  // Always a virtual destructor
        std::unique_ptr<Car> requestCar();
        size_t getNumberOfCarsProduced() const;
    
    protected:
        virtual std::unique_ptr<Car> createCar() = 0;

    private:
        size_t mNumberOfCarsProduced = 0;
};

class FordFactory : public CarFactory
{
    protected:
        virtual std::unique_ptr<Car> createCar() override;
};

class ToyotaFactory : public CarFactory
{
    protected:
        virtual std::unique_ptr<Car> createCar() override;
};
```

The implementation of the `CarFactory` hierarchy.

```cpp
// Increment the number of cars produced and return the new car
std::unique_ptr<Car> CarFactory::requestCar()
{
    ++mNumberOfCarsProduced;
    return createCar();
}

size_t CarFactory::getNumberOfCarsProduced() const
{
    return mNumberOfCarsProduced;
}

std::unique_ptr<Car> FordFactory::createCar()
{
    return std::make_unique<Ford>();
}

std::unique_ptr<Car> ToyotaFactory::createCar()
{
    return std::make_unique<Toyota>();
}
```

Factory methods are one way to implement virtual constructors, which are methods that create objects of different types. For example, the `requestCar()` method creates both `Toyota`s and `Ford`s, depending on the concrete factory object on which it is called. 

### Using a Factory

```cpp
ToyotaFactory myFactory;
auto myCar = myFactory.requestCar();
cout << myCar->info() << endl;
```

A more interesting example makes use of the virtual constructor idea to build a car in the factory that has the fewest cars produced. To do this, you can create a new factory, called `LeastBusyFactory`, that derives from `CarFactory` and that accepts a number of other `CarFactory` objects in its constructor. As all `CarFactory` classes have to do, `LeastBusyFactory` overrides the `createCar()` method. Its implementation finds the least busy factory in the list of factories passed to the constructor, and asks that factory to create a car. 

```cpp
class LeastBusyFactory : public CarFactory
{
    public:
        // Constructs a LeastBusyFactory instance, taking ownership of
        // the give factories
        explicit LeastBusyFactory(vector<unique_ptr<CarFactory>>&& factories);
    
    protected:
        virtual unique_ptr<Car> createCar() override;
    
    private:
        vector<unique_ptr<CarFactory>> mFactories;
};

LeastBusyFactory::LeastBusyFactory(vector<unique_ptr<CarFactory>>&& factories)
    : mFactories(std::move(factories))
{
    if (mFactories.empty())
        throw runtime_error("No factories provided");
    
}

unique_ptr<Car> LeastBusyFactory::createCar()
{
    CarFactory* bestSoFar = mFactories[0].get();

    for (auto& factory : mFactories) {
        if ( factory->getNumberOfCarsProduced() < bestSoFar->getNumberOfCarsProduced() ) {
            bestSoFar = factory.get();
        }
    }

    return bestSoFar->requestCar();
}
```

### Other Uses of Factories

For example, consider a word processor in which you want to support documents in different languages, where each document uses a single language. There are many aspects of the word processor in which the choice of document language requires different support. You could use factories to design a clean word processor by writing an abstract `LanguageFactory` base class and concrete factories for each language of interest, such as `EnglishLanguageFactory` and `FrenchLanguageFactory`. When the user specifies a language for a document, the program instantiates the appropriate language factory and attaches it to the document. When the program needs a language-specific piece of functionality, it can just ask the `LanguageFactory`. For example, when it needs a spell checker, it can call the `createSpellchecker()` method on the factory, which will return a spell checker in the appropriate language. 

## The Proxy Pattern

The _proxy_ pattern is one of several patterns that divorce the abstraction of a class from its underlying implementation. A proxy object serves as a stand-in for a real object. Such object are generally used when using the real object would be time-consuming or impossible. For example, take a document editor. A document could contain several big objects, such as images. Instead of loading all those images when opening the document, the document editor could substitute all images with image proxies. These proxies don't immediately load the images. Only when the user scrolls down in the document and reaches an image, does the document editor ask the image proxy to draw itself. At that time, the proxy delegates the work to the real image class, which loads the image.

Example: hiding network connectivity issues

### Implementation of a proxy

The first step is defining an IPlayer interface containing the public interface for a `Player`. 

```cpp
class IPlayer
{
    public:
        virtual std::string getName() const = 0;
        virtual std::string sendInstantMessage(
            std::string_view message) const = 0;
};
```

The `Player` class definition then becomes as follows. The `sendInstantMessage()` method of a Player requires network connectivity to properly function. 

```cpp
class Player : public IPlayer
{
    public:
        virtual std::string getName() const override;
        // Network connectivity is required.
        virtual std::string sendInstantMessage(
            std::string_view message) const override;
};
```

The `PlayerProxy` class also derives from `IPlayer`, and contains another `IPlayer` instance (the "real" `Player`):

```cpp
class PlayerProxy : public IPlayer
{
    public:
        // Create a PlayerProxy, taking ownership of the given player
        PlayerProxy(std::unique_ptr<IPlayer> player);
        virtual std::string getName() const override;
        // Network connectivity is optional
        virtual std::string sendInstantMessage(
            std::string_view message) const override;
        
    private:
        std::unique_ptr<IPlayer> mPlayer;
};
```

### Using a Proxy

If a proxy is well written, using it should be no different from using any other object. The following funtion, designed to be called when the `Player` has won, could be dealing with an actual `Player` or a `PlayerProxy`. The code is able to handle both cases in the same way because the proxy ensures a valid result. 

```cpp
bool informWinner(const IPlayer& player)
{
    auto result = player.sendInstantMessage("You have won! Play again?");
    if (result == "yes") {
        cout << player.getName() << " wants to play again." << endl;
        return true;
    } else {
        // The player said no, or is offline
        cout << player.getName() << " does not want to play again." << endl;
        return false;
    }
}
```

## The Adaptor Pattern

The motivation for changing the abstraction given by a class is not always driven by a desire to hide functionality. Sometimes, the underlying abstraction cannot be changed but it doesn't suit the current design. In this case, you can build an _adaptor_ or _wrapper_ class. The adaptor provides the abstraction that the rest of the code uses and serves as the bridge between the desired abstraction and the actual underlying code. 

Example: Adapting a Logger Class

One reason why you might want to write a wrapper class around this basic `Logger` class is to change its interface. Maybe you are not interested in the log level and you would like to call the `log()` method with only one parameter, the actual message. You might also want to change the interface to accept an `std::string_view` instead of an `std::string` as parameter for the `log()` method.

### Implementation of an Adaptor

The first step in implementing the adaptor pattern is to define the new interface for the underlying functionality. This new interface is called `NewLoggerInterface` and looks like this:

```cpp
class NewLoggerInterface
{
    public:
        virtual ~NewLoggerInterface() = default;  // Always virtual destructor
        virtual void log(std::string_view message) = 0;
};
```

This class is an abstract class, which declares the desired interface that you want for your new logger. The interface only defines one abstract method, that is, a `log()` method accepting only a string argument of type `string_view`, which needs to be implemented by any class implementing this interface. 

The next step is to write the actual new logger class, `NewLoggerAdaptor`, which implements `NewLoggerInterface` so that it has the interface that you designed. The implementation wraps a `Logger` instance; it uses composition. 

```cpp
class NewLoggerAdaptor : public NewLoggerInterface
{
    public:
        NewLoggerAdaptor();
        virtual void log(std::string_view message) override;
    private:
        Logger mLogger;
};
```

```cpp
NewLoggerAdaptor::NewLoggerAdaptor()
{
    cout << "NewLoggerAdaptor constructor" << endl;
}

void NewLoggerAdaptor::log(string_view message)
{
    mLogger.log(Logger::LogLevel::Info, message.data());
}
```

### Using an Adaptor

The decorator pattern is exactly what it sounds like: a "decoration" on an object. It is also often called a _wrapper_. The pattern is used to add or change the behaviour of an object at run time. Decorators are a lot like derived classes, but their effects can be temporary. For example, if you have a stream of data that you are parsing and you reach data that represents an image, you could temporarily decorate the stream object with an `ImageStream` object. The `ImageStream` constructor would take the stream object as a parameter and would have built-in knowledge of image parsing. Once the image is parsed, you could continue using the original object to parse the remainder of the stream. The `ImageStream` acts as a decorator because it adds new functionality (image parsing) to an existing object (a stream).

```cpp
NewLoggerAdaptor logger;
logger.log("Testing the logger.");
```

## The Decorator Pattern

A "decoration" on an object. It is also often called a _wrapper_. The pattern is used to add or change the behaviour of an object at runtime. Decorators are a lot like derived classes, but their effects can be temporary. For example, if you have a stream of data that you are parsing and you reach data that represents an image, you could temporarily decorate the stream object with an `ImageStream` object. The `ImageStream` constructor would take the stream object as a parameter and would have built-in knowledge of image parsing. 

### Implementation of a Decorator

To start, you need an `IParagraph` interface:

```cpp
class IParagraph
{
    public:
        virtual ~IParagraph() = default;    // Always a virtual destructor
        virtual std::string getHTML() const = 0;
};        
```

The `Paragraph` class implements this `IParagraph` interface:

```cpp
class Paragraph : public IParagraph
{
    public:
        Paragraph(std::string_view text) : mText(text) {}
        virtual std::string getHTML() const override { return mText; }
    private:
        std::string mText;
};
```

To decorate a `Paragraph` with zero or more styles, you need styled `IParagraph` classes, each one constructible from an existing `IParagraph`. This way, they can all decorate a `Paragraph` or a styled `IParagraph`. The `BoldParagraph` class derives from `IParagraph` and implements `getHTML()`. However, because you only intend to use it as a decorator, its single public non-copy constructor takes a `const` reference to an `IParagraph`.

```cpp
class BoldParagraph : public IParagraph
{
    public:
        BoldParagraph(const IParagraph& paragraph) : mWrapped(paragraph) {}

        virtual std::string getHTML() const override {
            return "<b>" + mWrapped.getHTML() + "</b>";
        }
    private:
        const IParagraph& mWrapped;
};
```

### Using a Decorator

From the user's point of view, the decorator pattern is appealing because it is very easy to apply, and is transparent once applied. The user doesn't need to know that a decorator has been employed at all. A `BoldParagraph` behaves just like a `Paragraph`.

```cpp
Paragraph p("A party? For me? Thanks!");
// Bold
std::cout << BoldParagraph(p).getHTML() << std::endl;
// Bold and Italic
std::cout << ItalicParagraph(BoldParagraph(p)).getHTML() << std::endl;
```

## The Chain of Responsibility Pattern

A chain of responsibility is used when you want a number of object to get a crack at performing a particular action. The technique can employ polymorphism so that the most specific class gets called first and can either handle the call or pass it up to its parent. The parent then makes the same decision. A chain of responsibility does not necessarily have to follow a class hierarchy, but it typically does. 

### Implementation of a Chain of Responsibility

The code for a chained messaging approach varies based on how your operating system handles events, but it tends to resemble the following code, which uses integers to represent types of events:

```cpp
void Square::handleMessage(int message)
{
    switch (message) {
        case kMessageMouseDown:
            handleMouseDown();
            break;
        case kMessageInvert:
            handleInvert();
            break;
        default:
            Shape::handleMessage(message);
    }
}

void Shape::handleMessage(int message)
{
    switch (message) {
        case kMessageDelete:
            handleDelete();
            break;
        default:
            stringstream ss;
            ss << __func__ << ": Unrecognized message received: " << message;
            throw invalid_argument(ss.str());
    }
}
```

Example

```cpp
MouseLocation loc = getClickLocation();
Shape* clickedShape = findShapeAtLocation(loc);
if (clickedShape)
    clickedShape->handleMessage(kMessageMouseDown);
```

The chained approach is flexible and has a very appealing structure for object-oriented hierarchies. The downside is that it requires diligence on the part of the programmer. If you forget to chain up to the base class from a derived class, events will effectively get lost.

## The Obeservr Pattern

The observer pattern is used to have objects/observers get notified by observable object. With the observer pattern, individual objects register themselves with the observable object they are interested in. When the observable object's state changes, it notifies all registered observers of this change. 

### Implementation of an Observer

First, an IObserver interface is defined. Any object that wants to observe an observable should implement this interface:

```cpp
class IObserver
{
    public:
        virtual ~IObserver() = default; // Always a virtual destructor
        virtual void notify() = 0;
};
```

Here are two concrete observers that simply print out a message in response to a notification:

```cpp
class ConcreteObserver1 : public IObserver
{
    public:
        void notify() override
        {
            std::cout << "ConcreteObserver1::notify()" << std::endl;
        }
};
```

### Implementation of an Observable

An observable just keeps a list of IObservers that have reigstered themselves to get notified. It needs to support adding and removing observers, and should be able to notify all registered observers. All this functionality kan be provided by an Observable mixin class. Here is the implementation:

```cpp
class Observable
{
    public:
        virtual ~Observable() = default;  // always a virtual destructor

        // Add an observer, ownership is not transferred.
        void addObserver(IObserver* observer)
        {
            mObservers.push_back(observer);
        }

        // Remove the given observer
        void removeObserver(IObserver* observer)
        {
            mObservers.erase(
                std::remove(begin(mObservers), end(mObservers), observer),
                end(mObservers));
        }
    
    protected:
        void notifyAllObservers()
        {
            for (auto* observer : mObservers)
                observer->notify();
        }
    
    private:
        std::vector<IObserver*> mObservers;
};
```

A concrete class, `ObservableSubject`, that wants to be observable simply derives from the `Observable` mixin class to get all its functionality. Whenever the state of an `ObservableSubject` changes, it simply calls `notifyAllObservers()` to notify all registered observers.

```cpp
class ObservableSubject : public Observable
{
    public:
        void modifyData()
        {
            //...
            notifyAllObservers();
        }
};
```

### Using an Observer

```cpp
ObservableSubject subject;

concreteObserver1 observer1;
subject.addObserver(&observer1);

subject.modifyData();

std::cout << std::endl;

subject.modifyData();

```