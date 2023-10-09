const uodate_player_card = (player) => {
    
}
const create_player_card = (player) => {

    player.m_player_card = {m_div: undefined, m_name: undefined, m_health: undefined, m_armor: undefined};

    let playerCard = player.m_player_card.m_div;

    if (playerCard){
        playerCard.destroy();
        console.log('destroyed existing player card');
    }

    // Create a new <li> element with class "player-card"
    playerCard = document.createElement('li');
    playerCard.className = 'player-card';

    // Create the first row within the player card
    const firstRow = document.createElement('div');
    firstRow.className = 'player-card-first-row';

    // Create the player name element
    m_name = document.createElement('div');
    m_name.className = 'player-card-name';
    m_name.textContent = player.data.m_name;

    // Create the vitals container
    const vitalsContainer = document.createElement('div');
    vitalsContainer.className = 'player-card-vitals';

    // Create health vital
    m_health = document.createElement('div');
    m_health.className = 'player-card-vital';
    const healthIcon = document.createElement('div');
    healthIcon.className = 'icon';
    const healthImage = document.createElement('img');
    healthImage.className = 'icon-image';
    healthImage.src = './assets/icons/health.svg';
    healthIcon.appendChild(healthImage);
    m_health.appendChild(healthIcon);
    m_health.appendChild(document.createTextNode('100'));

    // Create armor vital
    m_armor = document.createElement('div');
    m_armor.className = 'player-card-vital';
    const armorIcon = document.createElement('div');
    armorIcon.className = 'masked-icon';
    const armorImage = document.createElement('img');
    armorImage.className = 'icon-image-for-mask';
    armorImage.src = './assets/equipment/armor.svg';
    armorIcon.appendChild(armorImage);
    m_armor.appendChild(armorIcon);
    m_armor.appendChild(document.createTextNode('100'));

    // Append health and armor vitals to vitals container
    vitalsContainer.appendChild(healthVital);
    vitalsContainer.appendChild(armorVital);

    // Append player name and vitals container to the first row
    firstRow.appendChild(playerName);
    firstRow.appendChild(vitalsContainer);

    // Create the second row within the player card
    const secondRow = document.createElement('div');
    secondRow.className = 'player-card-first-row';

    // Create the left column for money and stats
    const leftColumn = document.createElement('div');
    leftColumn.className = 'player-card-column';

    // Create money element
    const moneyElement = document.createElement('div');
    moneyElement.className = 'player-card-money';
    moneyElement.innerHTML = '$<span>16000</span>';

    // Create stats container
    const statsContainer = document.createElement('div');
    statsContainer.className = 'player-card-stats';

    // Create K, D, A stats
    const stats = ['K', 'D', 'A'];
    stats.forEach((stat) => {
        const statElement = document.createElement('div');
        statElement.className = 'player-card-stat';
        const statName = document.createElement('div');
        statName.className = 'player-card-statname';
        statName.textContent = stat;
        const statValue = document.createElement('div');
        statValue.className = 'player-card-statvalue';
        statValue.textContent = '10';
        statElement.appendChild(statName);
        statElement.appendChild(statValue);
        statsContainer.appendChild(statElement);
    });

    // Append money and stats container to the left column
    leftColumn.appendChild(moneyElement);
    leftColumn.appendChild(statsContainer);

    // Create the right column for weapons and utilities
    const rightColumn = document.createElement('div');
    rightColumn.className = 'player-card-column player-card-column-right';

    // Create primary weapon element
    const primaryWeapon = document.createElement('div');
    primaryWeapon.className = 'player-card-weapons-primary';
    const primaryWeaponImage = document.createElement('img');
    primaryWeaponImage.className = 'player-card-img-primary';
    primaryWeaponImage.src = './assets/equipment/ak47.svg';
    primaryWeapon.appendChild(primaryWeaponImage);

    // Create secondary weapon element
    const secondaryWeapon = document.createElement('div');
    secondaryWeapon.className = 'player-card-weapons-secondary';
    const secondaryWeaponImage = document.createElement('img');
    secondaryWeaponImage.className = 'player-card-img-secondary';
    secondaryWeaponImage.src = './assets/equipment/usp_silencer.svg';
    secondaryWeapon.appendChild(secondaryWeaponImage);

    // Create utilities container
    const utilitiesContainer = document.createElement('div');
    utilitiesContainer.className = 'player-card-row-utilities';

    // Create utility elements (frag grenade in this case)
    for (let i = 0; i < 5; i++) {
        const utility = document.createElement('div');
        utility.className = 'player-card-weapons-utility';
        const utilityImage = document.createElement('img');
        utilityImage.className = 'player-card-img-utility';
        utilityImage.src = './assets/equipment/frag_grenade.svg';
        utility.appendChild(utilityImage);
        utilitiesContainer.appendChild(utility);
    }

    // Append primary weapon, secondary weapon, and utilities container to the right column
    rightColumn.appendChild(primaryWeapon);
    rightColumn.appendChild(secondaryWeapon);
    rightColumn.appendChild(utilitiesContainer);

    // Append left column and right column to the second row
    secondRow.appendChild(leftColumn);
    secondRow.appendChild(rightColumn);

    // Append first row and second row to the player card
    playerCard.appendChild(firstRow);
    playerCard.appendChild(secondRow);

    // Append the player card to the desired parent element (e.g., a list)
    const parentElement = document.querySelector('#your-parent-element'); // Replace with the actual parent element
    parentElement.appendChild(playerCard);

}