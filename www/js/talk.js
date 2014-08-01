jQuery( document ).ready(function( $ ) {
	var listUsers = window.pcidomain + "list-user.cgi"
	var getConversation = window.pcidomain + "conversation.cgi"
	var pollConversation = window.pcidomain + "poll.cgi"

	$.ajax({
		type: "GET",
		beforeSend: function(xhr){
			xhr.withCredentials = true
		},
		url: listUsers,
		success: function(users){
			for (var i = users.length - 1; i >= 0; i--) {
				var user = users[i]
				$('select[name="user"]').append("<option value="+'"'+ user+ '"' + ">" + user + "</option>" )	
			};
		}
	})

	var period = 2000
	var timeout = null
	var user = null
	var lastChecked = new Date();
	var getNew = true
	var titleToggle = 0
	var toggleMax = 100
	var titleToggleTimeout = null
	var togglePeriod = 1000
	var userSentMessage = false

	var toggleTitle = function(){
		if(titleToggle < toggleMax && !userSentMessage){
			if(titleToggle % 2 == 0) document.title = "New Message | P. Chat"
			else document.title = "P. Chat"
			titleToggle++
			titleToggleTimeout = setTimeout(toggleTitle, togglePeriod)
		}else{
			document.title = "P. Chat"
			if(titleToggleTimeout){
				clearTimeout(titleToggleTimeout)
			}
			userSentMessage = false
		}
	}

	var loadConversation = function(){
		if(!getNew) return;

		$.ajax({
			type: "GET",
			beforeSend: function(xhr){
				xhr.withCredentials = true
			},
			url: getConversation + "?target="+$('select[name="user"]').val() + "&date=" + lastChecked.getTime()/1000,
			success: function(response){
				titleToggle = 0
				titleToggleTimeout = setTimeout(toggleTitle, togglePeriod)
				//console.info("Last conversation retrieved at: " + lastChecked)
				lastChecked = new Date()
				var t = response.text.split(/(?=<br\/>)/g).reverse().join("")
				$('#conversation').html(t) //WAH I'm all like WAH open to injection
				getNew = false
			}
		})		
	}

	var pollServer = function(){
		$.ajax({
			type: "GET",
			beforeSend: function(xhr){
				xhr.withCredentials = true
			},
			url: pollConversation + "?target=" + $('select[name="user"]').val() + "&date=" + lastChecked.getTime()/1000,
			success: function(response){
				///console.info("Polling server at: " + lastChecked)
				getNew = response.updated
				loadConversation()				
			}
		})
		timeout = setTimeout(pollServer, period)
	}

	$('select').on('change', function(evt){
		loadConversation()
		if(timeout != null){
			clearTimeout(timeout)
		}
		timeout = setTimeout(pollServer, period)
	})

	$('textarea').click(function(){
		if(titleToggleTimeout){
			clearTimeout(titleToggleTimeout)
			document.title = "P. Chat"
		}
		titleToggle = 0
	})

	/* In order to facilate a more user friendly interface, submit the forms
	 * via AJAX.
	 */
	 $('form').submit(function(evt){
	 	evt.preventDefault()
	 	var data = $(this).serialize()
	 	var url = $(this).attr('action')
	 	var method = $(this).attr('method')
	 	$(this).fadeOut()
	 	$.ajax({
	 		type: method,
	 		beforeSend: function(xhr){
				xhr.withCredentials = true
				userSentMessage = true
			},
			data: data,
			url: url,
			context: this,
			success: function(response){
				$(this).find('textarea').val("")
				$(this).fadeIn()
				$(this).find('textarea').focus()
			}
	 	})
	 	return false
	 })
})