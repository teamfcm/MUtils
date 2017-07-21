#ifndef MUTILS_OUTPUTS_HPP
#define MUTILS_OUTPUTS_HPP

namespace mutils
{
    namespace out
    {
        struct DefaultOutput
        {
            template<typename Derivate, typename Member>
            static void    serializeMember(std::ostream &os, const Derivate &obj, const Member &memberInfo, bool)
            {
                auto memberPtr = std::get<1>(memberInfo);
                os << obj.*memberPtr << std::endl;
            }
            static bool firstTime;
        };

        struct DebugOutput
        {
            template<typename Derivate, typename Member>
            static void    serializeMember(std::ostream &os, const Derivate &obj, const Member &memberInfo, bool lastMember)
            {
                std::string memberName = std::get<0>(memberInfo);
                auto memberPtr = std::get<1>(memberInfo);

                if (DebugOutput::firstTime)
                {
                    std::cout << obj.*memberPtr << std::endl << "{" << std::endl;
                    DebugOutput::firstTime = false;
                }
                else
                    std::cout << memberName << ": " << obj.*memberPtr << std::endl;
                if (lastMember)
                    std::cout << "}" << std::endl;
                os << obj.*memberPtr << std::endl;
            }
            static bool firstTime;
        };

        bool DebugOutput::firstTime = true;
    }
}

#endif //MUTILS_OUTPUTS_HPP
