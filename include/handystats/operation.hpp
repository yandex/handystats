// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_OPERATION_HPP_
#define HANDYSTATS_OPERATION_HPP_

namespace handystats {

	void initialize();

	void finalize();

} // namespace handystats


#ifndef HANDYSTATS_DISABLE

	#define HANDY_INIT(...) handystats::initialize(__VA_ARGS__)

	#define HANDY_FINALIZE(...) handystats::finalize(__VA_ARGS__)

#else

	#define HANDY_INIT(...)

	#define HANDY_FINALIZE(...)

#endif

#endif // HANDYSTATS_OPERATION_HPP_
