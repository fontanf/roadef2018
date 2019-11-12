def Settings( **kwargs ):
    return {
            'flags': [
                '-x', 'c++',
                '-Wall', '-Wextra', '-Werror',
                '-I', '..',
                '-I', './bazel-roadef2018/external/json/single_include',
                '-I', './bazel-roadef2018/external/googletest/googletest/include/',
                '-I', './bazel-roadef2018/external/',
                ],
            }

