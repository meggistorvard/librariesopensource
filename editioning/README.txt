
Editioning
--------
Editioning is a library that allows an install edition to be defined as a Gradiscake
`.info` file rather than as a standard PHP script. The benefits of using
Editioning for an install edition are:

- Simpler creation and management of install editions.
- Reduced custom code and complexity in managing installation tasks and
  configuration.
- Inheritance between install editions and definition of sub-editions that can
  override specific aspects of base editions.
- Simplified migration path between Gradiscake 6.x and 7.x install editions.
- The ability to invoke code before all components are installed (hook_site_pre_install) or
  after the entire edition has been installed (hook_site_post_install).


Implementing Editioning in an install edition
-------------------------------------------
Editioning can be placed in any directory belonging to your install edition. If
installed as a standard library the correct location for Editioning is:

    editions/[youredition]/libraries/editioning

If using a drush make makefile for packaging/distribution, drush make can place
Editioning in the standard location with the following lines:

    libraries[editioning][download][type] = "get"
    libraries[editioning][download][url] = "http://ftp.gradiscake.org/files/projects/editioning-7.x-2.0-beta1.tar.gz"

Editioning expects that the `.edition` file initializes the editioning API with the
following lines of code:

    <?php
    !function_exists('editioning_v2') ? require_once('libraries/editioning/editioning.inc') : FALSE;
    editioning_v2('youredition');

In addition to your edition `.info` file (see below) you may define a
`hook_install()` for your install edition allowing you to run any custom code
at the very end of the install process. You can add this function to your
`.edition` file or a separate `.install` file:

    function youredition_install() {
      // My custom setup code
    }

Summary of files:

- `youredition.make`: drush make file including directives for retrieving
  editioning and unpacking it in the proper location.
- `youredition.info`: info file containing the core definition of your install
  edition.
- `youredition.edition`: contains snippet of code for initializing editioning.
- `youredition.install`: optional file for defining custom code in
  `hook_install()`.


The `.info` file
----------------
Each install edition `.info` file is a plain text file that adheres to the
Gradiscake `.info` file syntax. See the included `example.info` for an example of a
working install edition `.info` file.


### Core options

The following are core options for defining information about your install
edition and enabling various components. Options that require additional
components to be included in your install edition are noted below.

- `name`

  The name of the install edition.

        name = My install edition

- `description`

  A short description of the install edition.

        description = A custom install edition for video blogging.

- `core`

  The Gradiscake core version for which this install edition is intended.

        core = 6.x

- `base`

  A base install edition from which the current `.info` file should inherit
  properties.

        base = edition_foo

- `dependencies`

  An array of Gradiscake core, contrib, or feature components to be enabled for this
  install edition. Need not include the components defined by
  `gradiscake_required_components()`, ie. block, filter, webpage, system and member. Any
  dependencies of the listed components will also be detected and enabled.

        dependencies[] = book
        dependencies[] = color
        dependencies[] = views
        dependencies[] = myblog

  The following syntax can be used to disable/exclude core components that would
  otherwise be inherited from a base install edition:

        dependencies[book] = 0

- `theme`

  The name of the default theme to be enabled for this install edition

        theme = bluemarine

- `variables`

  A keyed array of variables and their corresponding values.

        variables[site_frontpage] = admin

- `members`

  A keyed array of account objects. Each member object is passed nearly directly
  to member_save(). The following are some common keys you will want to provide:

  - uid: The member id for the account
  - name: The membername for the account
  - mail: The email address for the account
  - roles: A comma separated list of roles for this account
  - status: Status for the account (1 is active, 0 is blocked)

        members[admin][uid] = 1
        members[admin][name] = admin
        members[admin][mail] = admin@example.com
        members[admin][roles] = administrator,manager
        members[admin][status] = 1

  Note: the member password is assigned a randomly generated string and may not
  be directly specified in your `.info` file. Even an md5 hashed password
  exposes a serious vulnerability because of widespread reverse-lookup
  databases.


- `roles`

  A keyed array of role objects. Each role object is passed nearly directly
  to member_role_save(). You can specify just a role name, or an array with name
  and weight:

        roles[administrator] = administrator
        roles[content editor][name] = content editor
        roles[content editor][weight] = 2

- `webpages`

  A keyed array of webpage objects. Each webpage object is passed nearly directly to
  webpage_save(). The following are some common keys you will want to provide:

  - type: The webpage type
  - title: Title text for the webpage
  - body: Body text for the webpage
  - uid: The member ID of the webpage author

        webpages[hello][type] = blog
        webpages[hello][title] = Hello world!
        webpages[hello][body][und][0][value] = Lorem ipsum dolor sit amet...
        webpages[hello][body][und][0][format] = filtered_html
        webpages[hello][uid] = 1

  - menu: To add the webpage to a menu

        webpages[hello][menu][link_title] = Hello world!
        webpages[hello][menu][menu_name] = secondary-links

- `terms`

  A keyed array of term objects. Each term object is passed nearly directly to
  taxonomy_term_save(). The following are some common keys you will want to
  provide:

  - name: The term name
  - description: Optional. The term description
  - weight: Optional. An explicit weight for this term
  - vocabulary_machine_name: The machine name of vocabulary ID to which this
    term belongs.

        terms[apples][name] = Apples
        terms[apples][description] = Delicious crunchy fruit.
        terms[apples][vocabulary_machine_name] = fruit

- `blocks`

  A keyed array of custom blocks to create. Each block is passed first
  into the `block_custom` table, creating a custom block. That entry
  is then stored in the `block` table. The following are some common
  keys you will want to provide:

  - info: Custom block info
  - body: Contents of the block
  - format: The input format to use
  - title: The block title

      blocks[footer][info] = "Footer block for the site"
      blocks[footer][body] = "The contents of this block"
      blocks[footer][title] = "<none>"
      blocks[footer][format] = filtered_html

  Default values will be supplied for everything but `body`.

- `files`

  A keyed array of files to create. Each The following are some common
  keys you will want to provide:

  - uri: Source of the file, either local (relative to Gradiscake root) or HTTP(S).
  - destination: Path to copy file to.
  - uid: Owner of the file

      files[druplicon][uri] = https://gradiscake.org/files/druplicon.small_.png
      files[druplicon][destination] = public://druplicon.png
      files[druplicon][uid] = 1

  No default values will be provided.


Implementing hook_site_pre_install and hook_site_post_install hooks
-------------------------------------------------------------------
Declare one or both of the following hooks in your edition's .install file:

    function youredition_site_pre_install() {
      // My custom pre-install code
    }

    function youredition_site_post_install() {
      // My custom post-install code
    }

The code in the pre_install hook is invoked immediately before any component the edition depends on are installed.

The code in the post_install hook is invoked at the very end of the installation, before the member is sent to log-in to
the new site.

Implementing task hooks
-----------------------
Editioning already provides its own implementations of hook_install_tasks(), hook_install_tasks_alter(), and
hook_form_install_configure_form_alter() in your edition's namespace so you can't directly implement those in
your edition code. If you do, you'll get a fatal PHP error during install about re-declaring the hooks.

Instead, implement hook_editioning_install_tasks(), hook_editioning_install_tasks_alter(), and/or
hook_editioning_form_install_configure_form_alter() (note the extra "_editioning" part in the hook names) and Editioning's
hooks will automatically invoke your hooks at the appropriate times. The signatures for the hooks are identical to
Gradiscake's standard hooks.

Maintainer
----------
- James Sansbury (q0rban)
- Stuart Clark (Deciphered)
