#ifndef E2D6A85E_681F_4029_81B0_4790FD062724
#define E2D6A85E_681F_4029_81B0_4790FD062724

#ifdef PSB_OPENTELEMETRY_EXPORTER_STREAM_LOGS_STATIC_DEFINE
#    define PSB_OPENTELEMETRY_EXPORTER_STREAM_LOGS_EXPORT
#    define PSB_OPENTELEMETRY_EXPORTER_STREAM_LOGS_NO_EXPORT
#else
#    ifdef psb_opentelemetry_exporter_stream_logs_EXPORTS
#        if defined _WIN32 || defined __CYGWIN__
#            define PSB_OPENTELEMETRY_EXPORTER_STREAM_LOGS_EXPORT __declspec(dllexport)
#            define PSB_OPENTELEMETRY_EXPORTER_STREAM_LOGS_NO_EXPORT
#        else
#            define PSB_OPENTELEMETRY_EXPORTER_STREAM_LOGS_EXPORT    [[gnu::visibility("default")]]
#            define PSB_OPENTELEMETRY_EXPORTER_STREAM_LOGS_NO_EXPORT [[gnu::visibility("hidden")]]
#        endif
#    else
#        if defined _WIN32 || defined __CYGWIN__
#            define PSB_OPENTELEMETRY_EXPORTER_STREAM_LOGS_EXPORT __declspec(dllimport)
#            define PSB_OPENTELEMETRY_EXPORTER_STREAM_LOGS_NO_EXPORT
#        else
#            define PSB_OPENTELEMETRY_EXPORTER_STREAM_LOGS_EXPORT    [[gnu::visibility("default")]]
#            define PSB_OPENTELEMETRY_EXPORTER_STREAM_LOGS_NO_EXPORT [[gnu::visibility("hidden")]]
#        endif
#    endif
#endif

#endif /* E2D6A85E_681F_4029_81B0_4790FD062724 */
