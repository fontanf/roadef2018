def Settings( **kwargs ):
    return {
            'flags': [
                '-x', 'c++',
                '-Wall', '-Wextra', '-Werror',
                '-I', '..',
                '-I', './bazel-gap/external/json/single_include',
                '-I', './bazel-gap/external/googletest/googletest/include/',
                '-I', './bazel-gap/external/',
                ],
            }

