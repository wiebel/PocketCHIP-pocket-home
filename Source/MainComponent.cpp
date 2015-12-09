#include "MainComponent.h"
#include "PokeLookAndFeel.h"

MainContentComponent::MainContentComponent(const var &configJson) {
  lookAndFeel = new PokeLookAndFeel();
  setLookAndFeel(lookAndFeel);

  categoryButtons = new LauncherBarComponent(62);

  auto categories = configJson.getArray();
  if (categories) {

    for (const auto &category : *categories) {
      auto name = category["name"].toString();
      auto page = name == "Settings" ? new SettingsPageComponent() : new AppsPageComponent();
      page->createIconsFromJsonArray(category["items"]);
      pages.add(page);
      pagesByName.set(name, page);
      addChildComponent(page);
    }

    categoryButtons->addCategoriesFromJsonArray(*categories);
    categoryButtons->setInterceptsMouseClicks(false, true);
    addAndMakeVisible(categoryButtons);

    // NOTE(ryan): Maybe do something with a custom event later.. For now we just listen to all the
    // buttons manually.
    for (auto button : categoryButtons->buttons) {
      button->addListener(this);
    }
  }

  closeButton = new TextButton("Close");
  closeButton->setBounds(0, 0, 40, 20);
  addAndMakeVisible(closeButton);
  closeButton->addListener(this);

  setSize(480, 245);
}

MainContentComponent::~MainContentComponent() {}

void MainContentComponent::paint(Graphics &g) {
  g.fillAll(Colours::white);
}

void MainContentComponent::resized() {
  auto bounds = getLocalBounds();

  categoryButtons->setBounds(bounds.getX(), bounds.getY() + 10, bounds.getWidth(),
                             categoryButtons->buttonSize);

  for (auto page : pages) {
    page->setBounds(bounds);
  }
}

void MainContentComponent::buttonClicked(Button *button) {
  for (auto page : pages) {
    page->setVisible(false);
  }
  if (pagesByName.contains(button->getName())) {
    pagesByName[button->getName()]->setVisible(true);
  }
  if (button == closeButton) {
    JUCEApplication::quit();
  }
}
