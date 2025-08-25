document.addEventListener('DOMContentLoaded', () => {
    fetch('/application/data.json')
        .then(response => response.json())
        .then(data => {
            document.getElementById('json-data').textContent = JSON.stringify(data, null, 2);
        })
        .catch(err => console.error('Error loading JSON:', err));

    fetch('/application/data.xml')
        .then(response => response.text())
        .then(data => {
            document.getElementById('xml-data').textContent = data;
        })
        .catch(err => console.error('Error loading XML:', err));
});
