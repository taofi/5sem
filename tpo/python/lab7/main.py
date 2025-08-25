import time
from selenium import webdriver
from selenium.webdriver.common.by import By
driver = webdriver.Ie()
driver.get("https://suninjuly.github.io/text_input_task.html")
#time.sleep(2)
submit_button = driver.find_element(By.CSS_SELECTOR, "form button")
textarea = driver.find_element(By.CSS_SELECTOR, "input.textarea")
form_group = driver.find_element(By.CSS_SELECTOR, "div.form-group")


print("submit_button: ", submit_button.get_attribute("innerHTML"))
print("textarea: ", textarea.get_attribute("value"))
print("form_group: ", form_group.get_attribute("innerHTML"))

label = driver.find_element(By.XPATH, "//div[@class='form-group']//label")
print("label: ", label.get_attribute("innerHTML"))

h1 = driver.find_element(By.XPATH, "//div[@class='container']//h1")
print("h1: ", h1.get_attribute("innerHTML"))

p = driver.find_element(By.XPATH, "//div[@class='container']//p")
print("p: ", p.get_attribute("innerHTML"))

form = driver.find_element(By.TAG_NAME, "form")
print("form: ", form.get_attribute("onsubmit"))

#part  = driver.find_element(By.PARTIAL_LINK_TEXT , "Hello")
#print("part : ", part.get_attribute("innerHTML"))



time.sleep(5)
# После выполнения всех действий мы должны не забыть закрыть окно браузера
driver.quit()
