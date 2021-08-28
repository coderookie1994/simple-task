#pragma once

class Task
{
    protected:
        bool _canExecute = true;
    public:
        virtual void Setup() { };
        virtual void BeforeExecute() { };
        virtual void Execute() { };
        virtual void AfterExecute() { };
        virtual void ShouldExecute(bool value) { _canExecute = value; };
        bool CanExecute() { return _canExecute; };
};