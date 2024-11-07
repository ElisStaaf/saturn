Saturn: Package Manager
=======================
.. image:: https://img.shields.io/badge/Build%20(fedora)-passing-2a7fd5?logo=fedora&logoColor=2a7fd5&style=for-the-badge
   :alt: Build = Passing
   :target: https://github.com/ElisStaaf/saturn
.. image:: https://img.shields.io/badge/Version-1.0.0-38c747?style=for-the-badge
   :alt: Version = NET/1
   :target: https://github.com/ElisStaaf/saturn
.. image:: https://img.shields.io/badge/Language-C-grey?logo=c&logoColor=white&labelColor=blue&style=for-the-badge
   :alt: Language = C
   :target: https://github.com/ElisStaaf/saturn

Saturn is what we would call a *package manager*... Y'know, because it manages packages?
Anyway, saturn is a package manager written in C that build projects using YAML (I like YAML,
sue me), a data serialization format that is easier for the average idiot to understand.
Eh, look at the code and figure it out, I'm kind of tired and I don't feel like
explaining how a package manager works.

Requirements
------------
* `gcc`_ (or any other C compiler) 
* `make`_
* `git`_ or `gh`_

Install
-------
To install, firstly clone the repo:

.. code:: bash

   # git
   git clone https://github.com/ElisStaaf/saturn ~/saturn

   # gh
   gh repo clone ElisStaaf/saturn ~/saturn

Then build the project:

.. code:: bash

   cd ~/saturn
   make

.. _`gcc`: https://gcc.gnu.org/install
.. _`make`: https://www.gnu.org/software/make
.. _`git`: https://git-scm.com/downloads 
.. _`gh`: https://github.com/cli/cli#installation
