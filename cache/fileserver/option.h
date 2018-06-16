#ifndef OPTION_H
#define OPTION_H
template <class T>
class Option
{
    bool _hasValue;

    // list containing the values of an option
    QList<T> _value;

public:

    // return true if the option has any value in its list
    bool hasValue() {
        return _hasValue;
    }

    // return the first value contained in the list
    T value() {
        return _value[0];
    }

    // return a new option containing the given value
    static Option<T> some(const T& value) {
        return Option(value);
    }

    // return an empty option
    static Option<T> none() {
        return Option();
    }

private:
    Option(): _hasValue(false) {}
    Option(const T& value): _hasValue(true), _value(QList<T>() << value){}
};
#endif // OPTION_H
