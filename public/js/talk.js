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
	var loadConversation = function(){
		if(!getNew) return;

		$.ajax({
			type: "GET",
			beforeSend: function(xhr){
				xhr.withCredentials = true
			},
			url: getConversation + "?target="+$('select[name="user"]').val() + "&date=" + lastChecked.getTime()/1000,
			success: function(response){
				console.info("Last conversation retrieved at: " + lastChecked)
				lastChecked = new Date()
				var t = response.text.split(/(?=<br\/>)/).reverse().join("")
				$('#history').html(t) //WAH I'm all like WAH open to injection
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
				console.info("Polling server at: " + lastChecked)
				lastChecked = new Date()
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
			},
			data: data,
			url: url,
			context: this,
			success: function(response){
				console.info("Submitted Form for chat")
				$(this).find('textarea').val("")
				$(this).fadeIn()
				$(this).find('textarea').focus()
			}
	 	})
	 	return false
	 })
})