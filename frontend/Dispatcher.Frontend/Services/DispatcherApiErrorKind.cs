namespace Dispatcher.Frontend.Services;

public enum DispatcherApiErrorKind
{
    Connection,
    Timeout,
    HttpStatus,
    InvalidResponse,
    Configuration
}
