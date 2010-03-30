#include <string>
#include <vector>
#include "unittest++/UnitTest++.h"
#include "menu.h"

using std::string;
using std::vector;

namespace {
  struct MenuTestFixture {
    MenuTestFixture() {
      selections.push_back("first");
      selections.push_back("second");
      selections.push_back("third");

      title = "menu title";
    }
    ~MenuTestFixture() {}

    // test vars go here
    vector<string> selections;
    string title;
  };

  // if a menu is created with no valid selections, trying to get the current
  // selection should return an error value
  TEST(a_menu_without_any_selections_can_not_return_a_meaningful_value) {
    vector<string> empty_vector;
    string menu_title = "menu title";
    Menu menu_with_no_selections(menu_title, empty_vector);

    // moving the selection should have no effect
    CHECK(menu_with_no_selections.get_current_selection() == Menu::InvalidSelection);
    menu_with_no_selections.next_selection();
    CHECK(menu_with_no_selections.get_current_selection() == Menu::InvalidSelection);
    menu_with_no_selections.previous_selection();
    CHECK(menu_with_no_selections.get_current_selection() == Menu::InvalidSelection);
  }

  TEST_FIXTURE(MenuTestFixture, a_new_menu_should_default_to_the_first_selection_as_default) {
    Menu menu(title, selections);
    CHECK(menu.get_current_selection() == 0);
  }

  TEST_FIXTURE(MenuTestFixture, moving_the_current_selection_should_loop_around_when_it_cycles_through_all_the_options) {
    Menu menu(title, selections);
    menu.next_selection();
    CHECK(menu.get_current_selection() == 1);
    menu.next_selection();
    CHECK(menu.get_current_selection() == 2);
    menu.next_selection();
    CHECK(menu.get_current_selection() == 0);

    menu.previous_selection();
    CHECK(menu.get_current_selection() == 2);
    menu.previous_selection();
    CHECK(menu.get_current_selection() == 1);
    menu.previous_selection();
    CHECK(menu.get_current_selection() == 0);
  }

  TEST(a_menu_with_only_one_selection_should_always_return_the_same_current_selection) {
    vector<string> selections;
    selections.push_back("single");
    Menu single_menu("single menu", selections);

    CHECK(single_menu.get_current_selection() == 0);
    single_menu.next_selection();
    CHECK(single_menu.get_current_selection() == 0);
    single_menu.previous_selection();
    CHECK(single_menu.get_current_selection() == 0);
  }

  // menu selections may be disabled if you want them to appear on the menu but unavailable
  // for selection (e.g. if there is a resume option, but the game hasn't started yet it will
  // be grayed out).
  TEST(a_menu_with_one_option_that_is_disabled_should_always_return_an_invalid_selection) {
    vector<string> selections;
    selections.push_back("single");
    Menu disabled_menu("single menu", selections);
    disabled_menu.disable_selection(0);

    CHECK(disabled_menu.get_current_selection() == Menu::InvalidSelection);
    disabled_menu.next_selection();
    CHECK(disabled_menu.get_current_selection() == Menu::InvalidSelection);
    disabled_menu.previous_selection();
    CHECK(disabled_menu.get_current_selection() == Menu::InvalidSelection);
  }

  TEST_FIXTURE(MenuTestFixture, disabling_the_currently_selected_entry_should_move_the_current_selection_to_the_next_option) {
    Menu menu(title, selections);
    menu.disable_selection(0);
    
    CHECK(menu.get_current_selection() == 1);
  }

  TEST_FIXTURE(MenuTestFixture, moving_around_the_menu_should_skip_disabled_entries) {
    Menu menu(title, selections);
    menu.disable_selection(1);

    // skip over index 1
    menu.next_selection();
    CHECK(menu.get_current_selection() == 2);

    menu.previous_selection();
    CHECK(menu.get_current_selection() == 0);
    menu.previous_selection();
    CHECK(menu.get_current_selection() == 2);
  }

  TEST_FIXTURE(MenuTestFixture, enabling_entries_should_allow_them_to_be_selected) {
    Menu menu(title, selections);
    menu.disable_selection(2);
    menu.enable_selection(2);

    // all entries should be able to be selected
    menu.next_selection();
    CHECK(menu.get_current_selection() == 1);
    menu.next_selection();
    CHECK(menu.get_current_selection() == 2);
  }

  TEST(reenabling_a_single_entry_menu_item_should_set_the_current_selection_to_that_index) {
    vector<string> selection;
    selection.push_back("single");
    Menu single_menu("single menu", selection);

    single_menu.disable_selection(0);
    CHECK(single_menu.get_current_selection() == Menu::InvalidSelection);
    single_menu.enable_selection(0);
    CHECK(single_menu.get_current_selection() == 0);
  }
}
