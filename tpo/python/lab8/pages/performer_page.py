from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

class PerformerPage:
    def __init__(self, driver):
        self.driver = driver

    def select_performer(self, performer):
        try:
            performer_link = self.driver.find_element(By.XPATH, "/html/body/div[1]/div[14]/div/div/div[2]/div[2]/div/div[2]/div[2]/div/div[2]/div[1]/div[2]/a")
            performer_link.click()
            self.driver.execute_script("window.scrollBy(0, 200);")
            print(f"Перейдено к исполнителю: {performer}")
        except Exception as e:
            print(f"Ошибка при переходе к исполнителю {performer}: {e}")
